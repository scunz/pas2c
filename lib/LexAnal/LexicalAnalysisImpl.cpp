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

#include <QStringBuilder>

#include "LexicalAnalysisImpl.hpp"

#define RAW_TOKEN(token,type) {token, type}
#define OPER_TOKEN(token,type) RAW_TOKEN(token, type)
#define TEXT_TOKEN(token,type) RAW_TOKEN(#token, type)

struct TokenDef {
    const char*     szTokenText;
    TokenType       type;
};

static TokenDef textTokens[] = {
    TEXT_TOKEN(begin,           T_BEGIN),
    TEXT_TOKEN(end,             T_END),
    TEXT_TOKEN(program,         T_PROGRAM),
    TEXT_TOKEN(unit,            T_UNIT),
    TEXT_TOKEN(interface,       T_INTERFACE),
    TEXT_TOKEN(implementation,  T_IMPLEMENTATION),
    TEXT_TOKEN(procedure,       T_PROCEDURE),
    TEXT_TOKEN(var,             T_VAR),
    TEXT_TOKEN(const,           T_CONST),
    TEXT_TOKEN(type,            T_TYPE),
    TEXT_TOKEN(function,        T_FUNCTION),
    TEXT_TOKEN(string,          T_STRING),
    TEXT_TOKEN(boolean,         T_BOOLEAN),
    TEXT_TOKEN(integer,         T_INTEGER),
    TEXT_TOKEN(pointer,         T_POINTER),
    TEXT_TOKEN(char,            T_CHAR),
    TEXT_TOKEN(float,           T_FLOAT),
    TEXT_TOKEN(text,            T_TEXT),
    TEXT_TOKEN(file,            T_FILE),
    TEXT_TOKEN(record,          T_RECORD),
    TEXT_TOKEN(array,           T_ARRAY),
    TEXT_TOKEN(set,             T_SET),
    TEXT_TOKEN(of,              T_OF),
    TEXT_TOKEN(if,              T_IF),
    TEXT_TOKEN(case,            T_CASE),
    TEXT_TOKEN(else,            T_ELSE),
    TEXT_TOKEN(then,            T_THEN),
    TEXT_TOKEN(for,             T_FOR),
    TEXT_TOKEN(while,           T_WHILE),
    TEXT_TOKEN(do,              T_DO),
    TEXT_TOKEN(repeat,          T_REPEAT),
    TEXT_TOKEN(until,           T_UNTIL),
    TEXT_TOKEN(to,              T_TO),
    TEXT_TOKEN(downto,          T_DOWNTO),
    TEXT_TOKEN(with,            T_WITH),
    TEXT_TOKEN(and,             T_AND),
    TEXT_TOKEN(or,              T_OR),
    TEXT_TOKEN(not,             T_NOT),
    TEXT_TOKEN(in,              T_IN),
    TEXT_TOKEN(as,              T_AS),
    TEXT_TOKEN(shl,             T_SHL),
    TEXT_TOKEN(shr,             T_SHR),
    TEXT_TOKEN(div,             T_DIV),
    TEXT_TOKEN(mod,             T_MOD),
    TEXT_TOKEN(uses,            T_USES),
    RAW_TOKEN(NULL,             T__ERROR)
};

static TokenDef operTokens[] = {
    // reverse sort by token text length
    OPER_TOKEN(":=", T_ASSIGNMENT),
    OPER_TOKEN("<=", T_LESS_THAN_EQUAL),
    OPER_TOKEN("=>", T_GREATER_THAN_EQUAL),
    OPER_TOKEN("<>", T_INEQUAL),
    OPER_TOKEN("..", T_ELLIPSIS),

    OPER_TOKEN("+", T_PLUS),
    OPER_TOKEN("-", T_MINUS),
    OPER_TOKEN("/", T_SLASH),
    OPER_TOKEN("*", T_ASTERIKS),
    OPER_TOKEN("<", T_LEFT_ANGLE),
    OPER_TOKEN(">", T_RIGHT_ANGLE),
    OPER_TOKEN("(", T_LEFT_PAREN),
    OPER_TOKEN(")", T_RIGHT_PAREN),
    OPER_TOKEN("[", T_LEFT_SQUARE),
    OPER_TOKEN("]", T_RIGHT_SQUARE),
    OPER_TOKEN("=", T_EQUAL),
    OPER_TOKEN(",", T_COMMA),
    OPER_TOKEN(":", T_COLON),
    OPER_TOKEN(".", T_DOT),
    OPER_TOKEN(";", T_SEMICOLON),
    OPER_TOKEN("@", T_AT_SIGN),
    OPER_TOKEN("^", T_CARET),
    RAW_TOKEN(NULL,         T__ERROR)
};

LexicalAnalyzerImpl::LexicalAnalyzerImpl() {
    mTokenStream = new TokenStreamImpl;
}

TokenType LexicalAnalyzerImpl::isTextToken(const char* token) {
    int pos = -1;
    while (textTokens[++pos].szTokenText) {
        if (!strcasecmp(textTokens[pos].szTokenText, token))
            break;
    }
    return textTokens[pos].type;
}

TokenType LexicalAnalyzerImpl::isOperToken(const char* token) {
    int pos = -1;
    while (operTokens[++pos].szTokenText) {
        if (!strcmp(operTokens[pos].szTokenText, token))
            break;
    }
    return operTokens[pos].type;
}

void LexicalAnalyzerImpl::setProject(Project::Ptr prj) {
    mProject = prj;
}

void LexicalAnalyzerImpl::setInput(const QString& fileName) {
    InputStream::Ptr is = mProject->inputStreamFor(fileName);
    mState = new State(is);
}

bool LexicalAnalyzerImpl::analyze() {
    Q_ASSERT(mState);

    while (mState) {
        if (eof()) {
            continue;
        }

        skipSpace();

        if (eof()) {
            continue;
        }

        char ch = mState->ch();
        if (ch == '{') {
            QByteArray comment;
            if (!skipBraceComment(comment)) {
                return false;
            }
            if (comment.startsWith('$')) {
                if (!preprocessor(comment)) {
                    return false;
                }
            }
            continue;
        }
        else if (ch == '(' && mState->chNext() == '*') {
            QByteArray comment;

            mState->mColumn += 2;
            mState->mPos += 2;

            if (!skipParenComment(comment)) {
                return false;
            }

            if (comment.startsWith('$')) {
                if (!preprocessor(comment)) {
                    return false;
                }
            }
            continue;
        }
        else if (ch == '$') {
            int c = mState->mColumn++;
            mState->mPos++;
            quint64 num = 0;
            ch = mState->ch();
            if (!((ch >= '0' && ch <= '9') ||
                  (ch >= 'A' && ch <= 'F') ||
                  (ch >= 'a' && ch <= 'f')) || eof()) {

                emitError(QLatin1String("Bad hex constant"), mkRef(c));
                return false;
            }
            do {
                num <<= 4;
                if (ch >= '0' && ch <= '9') {
                    num += ch - '0';
                }
                else if (ch >= 'A' && ch <= 'F') {
                    num += ch - 'A';
                }
                else if (ch >= 'a' && ch <= 'f') {
                    num += ch - 'a';
                }
                mState->mColumn++;
                mState->mPos++;
                if (eof()) {
                    return false;
                }
                ch = mState->ch();
            } while ((ch >= '0' && ch <= '9') ||
                     (ch >= 'A' && ch <= 'F') ||
                     (ch >= 'a' && ch <= 'f'));
            mTokenStream->appendToken(TokenImpl(mkRef(c), num));
            continue;
        }
        else if (ch >= '0' && ch <= '9') {
            int c = mState->mColumn;
            quint64 num = 0;
            do {
                num *= 10;
                num += ch - '0';
                mState->mColumn++;
                mState->mPos++;
                if (eof()) {
                    return false;
                }
                ch = mState->ch();
            } while (ch >= '0' && ch <= '9');

            if (ch == '.') {
                mState->mColumn++;
                mState->mPos++;
                if (!eof()) {
                    double dnum = num;
                    // ### FLOAT/DOUBLE parsing
                }
            }
            else {
                mTokenStream->appendToken(TokenImpl(mkRef(c), num));
            }
            continue;
        }
        else if ((ch == '_') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
            int c = mState->mColumn;
            QByteArray s;
            do {
                s += ch;
                mState->mColumn++;
                mState->mPos++;
                if (eof()) {
                    return false;
                }
                ch = mState->ch();
            } while ((ch == '_') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9'));

            TokenType tt = isTextToken(s.constData());
            if (tt == T__ERROR) {
                mTokenStream->appendToken(TokenImpl(mkRef(c), T__IDENTIFIER, s));
            }
            else {
                mTokenStream->appendToken(TokenImpl(mkRef(c), tt));
            }
            continue;
        }
        // ### '#'s are missing from string analyzing
        // ### '''' is missing from string analyzing
        else if (ch == '\'') {
            int c = mState->mColumn;
            QByteArray s;
            mState->mColumn++;
            mState->mPos++;

            if (eof()) {
                emitError(QLatin1String("Unfinished String"), mkRef(c));
                return false;
            }

            ch = mState->ch();
            do {
                if (ch == '\n') {
                    emitError(QLatin1String("Linebreak in String"), mkRef(c));
                    return false;
                }

                s += ch;

                mState->mColumn++;
                mState->mPos++;
                ch = mState->ch();

                if (eof()) {
                    emitError(QLatin1String("Unfinished String"), mkRef(c));
                    return false;
                }

            } while (ch != '\'');

            mTokenStream->appendToken(TokenImpl(mkRef(c), T__STRING, s));

            mState->mColumn++;
            mState->mPos++;
            continue;
        }
        else {
            int c = mState->mColumn;
            QByteArray s;
            do {
                s += ch;
                mState->mColumn++;
                mState->mPos++;
                if (eof()) { // oper tokens are right at EOF
                    break;
                }
                ch = mState->ch();
            } while ((ch != '_') && (ch != '{') && (ch != '(') && (ch > ' ') &&
                     !(ch >= 'a' && ch <= 'z') &&
                     !(ch >= 'A' && ch <= 'Z') &&
                     !(ch >= '0' && ch <= '9') );

            bool found = false;
            while (s.count() > 0) {
                TokenType tt = isOperToken(s.constData());

                if (tt != T__ERROR) {
                    mTokenStream->appendToken(TokenImpl(mkRef(c), tt));
                    found = true;
                    break;
                }
                mState->mPos--;
                mState->mColumn--;
                s = s.left(s.length()-1);
            }

            if (found) {
                continue;
            }

            emitError(QLatin1Literal("Found dizzy stray char '") %
                      QLatin1Char(ch) %
                      QChar(L'\''),
                      mkRef(c));
            return false;
        }
    }

    return true;
}

bool LexicalAnalyzerImpl::skipParenComment(QByteArray& comment) {
    while (!eof()) {
        char ch = mState->ch();
        switch(ch) {
        case '\n':
            comment += ch;
            mState->mLine++;
            mState->mColumn = 1;
            mState->mPos++;
            continue;

        case ' ':
        case '\t':
            mState->mColumn++;
        case '\r':
            mState->mPos++;
            comment += ch;
            continue;

        case '*':
            mState->mColumn++;
            mState->mPos++;
            if (!eof() && mState->ch() == ')') {
                mState->mColumn++;
                mState->mPos++;
                return true;
            }
            comment += ch;
            continue;

        default:
            comment += ch;
            mState->mColumn++;
            mState->mPos++;
            break;
        }
    }

    emitError(QLatin1String("Unfinished comment"), mkRef(mState->mColumn));
    return false;
}

bool LexicalAnalyzerImpl::skipBraceComment(QByteArray& out) {
    int start = mState->mPos + 1;

    while (mState->mPos < mState->mStream->size()) {
        switch(mState->ch()) {
        case '\n':
            mState->mLine++;
            mState->mColumn = 1;
            mState->mPos++;
            continue;

        case ' ':
        case '\t':
            mState->mColumn++;
        case '\r':
            mState->mPos++;
            continue;

        case '}':
            out = mState->mStream->data().mid(start, mState->mPos - start);
            mState->mColumn++;
            mState->mPos++;
            return true;

        default:
            mState->mColumn++;
            mState->mPos++;
            break;
        }
    }

    emitError(QLatin1String("Unfinished comment"), mkRef(mState->mColumn));
    return false;
}

bool LexicalAnalyzerImpl::eof() {
    bool isEof = mState->mPos >= mState->mStream->size();
    if (isEof) {
        State* p = mState->mPrev;
        delete mState;
        mState = p;
    }
    return isEof;
}

void LexicalAnalyzerImpl::skipSpace() {
    while (!eof()) {
        switch(mState->ch()) {
        case '\n':
            mState->mLine++;
            mState->mColumn = 1;
            mState->mPos++;
            continue;

        case ' ':
        case '\t':
            mState->mColumn++;
        case '\r':
            mState->mPos++;
            continue;

        default:
            return;
        }
    }
}

TokenStream::Ptr LexicalAnalyzerImpl::tokenStream() const {
    return mTokenStream;
}

bool LexicalAnalyzerImpl::preprocessor(const QByteArray& comment) {
    qDebug() << "Ignoring PP-Instruction: " << comment;
}

void LexicalAnalyzerImpl::emitError(const QString& text, const InputStreamRef& ref) {
    mProject->emitError(text, ref);
}

void LexicalAnalyzerImpl::emitError(const char* text, const InputStreamRef& ref) {
    mProject->emitError(text, ref);
}
