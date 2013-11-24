/*
 * Pas2CTrans - Transcoder for Pascal to plain old C code
 * Copyright (C) 2013 Sascha Cunz <sascha.cunz@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License (Version 2) as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if
 * not, see <http://www.gnu.org/licenses/>.
 *
 */

// Qt
#include <QFile>
#include <QTextStream>
#include <QStringBuilder>

// CoreUtils
#include "InputStream.hpp"

// LexAnal
#include "TokenImpl.hpp"

static inline QByteArray pascalEscapedString(const QByteArray& a) {
    QByteArray result;
    result.reserve(a.count());
    bool in = false;
    for (int i = 0; i < a.count(); i++) {
        unsigned char ch = a[i];
        if (ch < 32 || ch > 127) {
            if (in) {
                result += "'#" % QByteArray::number(ch);
                in = false;
            }
            else {
                result += "#" % QByteArray::number(ch);
            }
        }
        else if (in) {
            result += ch;
        }
        else {
            result += "'" + ch;
            in = true;
        }
    }
    if (in) {
        result += '\'';
    }
    return result;
}

#define TOKEN_NAME(t) { t, #t }
static struct TokenTypeNames {
    TokenType type;
    const char* name;
}
TokenTypeNames[] = {
    TOKEN_NAME(T_DOT),
    TOKEN_NAME(T_COMMA),
    TOKEN_NAME(T_SEMICOLON),
    TOKEN_NAME(T_COLON),
    TOKEN_NAME(T_LEFT_PAREN),
    TOKEN_NAME(T_RIGHT_PAREN),
    TOKEN_NAME(T_LEFT_ANGLE),
    TOKEN_NAME(T_RIGHT_ANGLE),
    TOKEN_NAME(T_PLUS),
    TOKEN_NAME(T_MINUS),
    TOKEN_NAME(T_ASTERIKS),
    TOKEN_NAME(T_SLASH),
    TOKEN_NAME(T_LEFT_SQUARE),
    TOKEN_NAME(T_RIGHT_SQUARE),
    TOKEN_NAME(T_AT_SIGN),
    TOKEN_NAME(T_CARET),
    TOKEN_NAME(T_EQUAL),

    TOKEN_NAME(T_ASSIGNMENT),
    TOKEN_NAME(T_LESS_THAN_EQUAL),
    TOKEN_NAME(T_GREATER_THAN_EQUAL),
    TOKEN_NAME(T_INEQUAL),
    TOKEN_NAME(T_ELLIPSIS),

    TOKEN_NAME(T_BEGIN),
    TOKEN_NAME(T_END),
    TOKEN_NAME(T_PROGRAM),
    TOKEN_NAME(T_INTERFACE),
    TOKEN_NAME(T_IMPLEMENTATION),
    TOKEN_NAME(T_UNIT),
    TOKEN_NAME(T_FUNCTION),
    TOKEN_NAME(T_PROCEDURE),
    TOKEN_NAME(T_VAR),
    TOKEN_NAME(T_CONST),
    TOKEN_NAME(T_TYPE),

    TOKEN_NAME(T_STRING),
    TOKEN_NAME(T_CHAR),
    TOKEN_NAME(T_BOOLEAN),
    TOKEN_NAME(T_POINTER),
    TOKEN_NAME(T_INTEGER),
    TOKEN_NAME(T_FLOAT),
    TOKEN_NAME(T_TEXT),
    TOKEN_NAME(T_FILE),

    TOKEN_NAME(T_RECORD),
    TOKEN_NAME(T_ARRAY),
    TOKEN_NAME(T_SET),

    TOKEN_NAME(T_OF),
    TOKEN_NAME(T_USES),

    TOKEN_NAME(T_IF),
    TOKEN_NAME(T_CASE),
    TOKEN_NAME(T_ELSE),
    TOKEN_NAME(T_THEN),
    TOKEN_NAME(T_FOR),
    TOKEN_NAME(T_WHILE),
    TOKEN_NAME(T_DO),
    TOKEN_NAME(T_REPEAT),
    TOKEN_NAME(T_UNTIL),
    TOKEN_NAME(T_TO),
    TOKEN_NAME(T_DOWNTO),
    TOKEN_NAME(T_WITH),

    TOKEN_NAME(T_AND),
    TOKEN_NAME(T_OR),
    TOKEN_NAME(T_NOT),
    TOKEN_NAME(T_IN),
    TOKEN_NAME(T_AS),
    TOKEN_NAME(T_SHL),
    TOKEN_NAME(T_SHR),
    TOKEN_NAME(T_DIV),
    TOKEN_NAME(T_MOD),

    TOKEN_NAME(T__STRING),
    TOKEN_NAME(T__NUMBER),
    TOKEN_NAME(T__FLOAT),
    //TOKEN_NAME(T__CHAR),
    TOKEN_NAME(T__IDENTIFIER),

    TOKEN_NAME(T__ERROR),
    TOKEN_NAME(T__END_OF_STREAM),
    { TokenType(-1), NULL}
};

const char* tokenName(TokenType t) {
    int i = -1;
    while (TokenTypeNames[++i].name) {
        if (TokenTypeNames[i].type == t) {
            return TokenTypeNames[i].name;
        }
    }

    return "<unknown Token>";
}

// --

TokenImpl::TokenImpl()
    : mType(T__ERROR)
{
}

TokenImpl::TokenImpl(const InputStreamRef& isr, TokenType type)
    : mStreamRef(isr)
    , mType(type)
{
}

TokenImpl::TokenImpl(const InputStreamRef& isr, quint64 num)
    : mStreamRef(isr)
    , mType(T__NUMBER)
    , mNumberInteger(num)
{
}

TokenImpl::TokenImpl(const InputStreamRef& isr, double num)
    : mStreamRef(isr)
    , mType(T__FLOAT)
    , mNumberDouble(num)
{
}

TokenImpl::TokenImpl(const InputStreamRef& isr, TokenType type, const QByteArray& text)
    : mStreamRef(isr)
    , mType(type)
{
    Q_ASSERT(type == T__STRING || type == T__IDENTIFIER);
    mTextData.length = text.length();
    mTextData.text = strdup(text.constData());
}

TokenImpl::TokenImpl(const TokenImpl& other) {
    assign(other);
}

TokenImpl& TokenImpl::operator=(const TokenImpl& other) {
    assign(other);
    return *this;
}

void TokenImpl::assign(const TokenImpl& other) {
    mStreamRef = other.mStreamRef;
    mType = other.mType;

    switch(mType) {
    case T__IDENTIFIER:
    case T__STRING:
        mTextData.length = other.mTextData.length;
        mTextData.text = strdup(other.mTextData.text);
        break;

    case T__NUMBER:
        mNumberInteger = other.mNumberInteger;
        break;

    case T__FLOAT:
        mNumberDouble = other.mNumberDouble;
        break;
    }

}

TokenImpl::~TokenImpl() {
    if (mType == T__IDENTIFIER || mType == T__STRING) {
        free(mTextData.text);
    }
}

QByteArray TokenImpl::name() const {
    return tokenName(mType);
}

QByteArray TokenImpl::description() const {
    switch (mType) {
    case T__IDENTIFIER:     return "Identifier(" % textDataRaw() % ")";
    case T__STRING:         return "String('" % pascalEscapedString(textDataRaw()) % "')";
    case T__NUMBER:         return "Number(" % QByteArray::number(mNumberInteger) % ")";
    case T__FLOAT:          return "FloatPoint(" % QByteArray::number(mNumberDouble) % ")";
    case T__END_OF_STREAM:  return "<EOS>";
    case T__ERROR:          return "<ERROR>";
    default:                return name();
    }
}

QByteArray TokenImpl::text() const {
    Q_ASSERT(mType == T__STRING);
    return textDataRaw();
}

QByteArray TokenImpl::identifier() const {
    Q_ASSERT(mType == T__IDENTIFIER);
    return textDataRaw();
}

quint64 TokenImpl::number() const {
    Q_ASSERT(mType == T__NUMBER);
    return mNumberInteger;
}

double TokenImpl::numberFloat() const {
    Q_ASSERT(mType == T__FLOAT);
    return mNumberDouble;
}

const InputStreamRef& TokenImpl::inputStreamRef() const {
    return mStreamRef;
}


QByteArray TokenImpl::textDataRaw() const {
    Q_ASSERT(mType == T__STRING || mType == T__IDENTIFIER);
    return QByteArray(mTextData.text, mTextData.length);
}

TokenType TokenImpl::type() const {
    return mType;
}

QDebug operator<<(QDebug dbg, const TokenImpl& token) {
    dbg.nospace() << "TokenImpl{"
                  << token.description().constData()
                  << "@"
                  << token.inputStreamRef()
                  << "}";
    return dbg.space();
}

