
#include "lexer/inputstream.hpp"
#include "lexer/token.hpp"

PtrT<InputStream> InputStreamRef::stream() const
{
    return mStream;
}

// --

Token::Token()
    : mType(T__ERROR)
{}

Token::Token(InputStreamRef isr, TokenType type)
    : mType(type)
{}

Token::Token(InputStreamRef isr, int num)
    : mType(T__NUMBER)
    , mNumberInteger(num)
{}

Token::Token(InputStreamRef isr, double num)
    : mType(T__FLOAT)
    , mNumberDouble(num)
{}

Token::Token(InputStreamRef isr, TokenType type, const QByteArray& text)
    : mType(type)
    , mText(text)
{}

Token::Token(const Token& other) {

    assign(other);
}

Token& Token::operator=(const Token& other) {
    assign(other);
    return *this;
}

void Token::assign(const Token& other) {
    mType = other.mType;

    switch(mType) {
    case T__IDENTIFIER:
    case T__STRING:
        mText = other.mText;
        break;

    case T__NUMBER:
        mNumberInteger = other.mNumberInteger;
        break;

    case T__FLOAT:
        mNumberDouble = other.mNumberDouble;
        break;
    }

}

Token::~Token() {
}

// -- Tokens ------------------------------------------------------------------------------------ >8

Tokens::Tokens()
    : mPos(-2)
{}

Tokens::~Tokens()
{}

Token Tokens::at(int pos) const {
    if (pos < mTokens.count()) {
        return mTokens.at(pos);
    }
    else {
        return Token(InputStreamRef(), T__END_OF_STREAM);
    }
}

void Tokens::init(const QVector<Token>& tokens)
{
    Q_ASSERT(mTokens.count() == 0);
    mTokens = tokens;
    mPos = -1;
}
