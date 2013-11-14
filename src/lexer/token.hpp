
#ifndef PASC_TOKEN_HPP
#define PASC_TOKEN_HPP

#include <QByteArray>
#include <QVector>

#include "common/ptr.hpp"

enum TokenType {

    T_DOT,
    T_COMMA,
    T_SEMICOLON,
    T_COLON,
    T_LEFT_PAREN,
    T_RIGHT_PAREN,
    T_LEFT_ANGLE,
    T_RIGHT_ANGLE,
    T_PLUS,
    T_MINUS,
    T_ASTERIKS,
    T_SLASH,
    T_LEFT_SQUARE,
    T_RIGHT_SQUARE,
    T_AT_SIGN,
    T_CARET,

    T_ASSIGNMENT,
    T_LESS_THAN_EQUAL,
    T_GREATER_THAN_EQUAL,
    T_INEQUAL,
    T_ELLIPSIS,

    T_BEGIN,
    T_END,
    T_PROGRAM,
    T_FUNCTION,

    T__STRING,
    T__NUMBER,
    T__FLOAT,
    T__CHAR,
    T__IDENTIFIER,

    T__ERROR,
    T__END_OF_STREAM
};

class InputStream;

class InputStreamRef
{
public:
    InputStreamRef()
        : mStream(NULL), mLine(-1), mCol(-1)
    {}

    InputStreamRef(InputStream* stream, int line, int column)
        : mStream(stream), mLine(line), mCol(column)
    {}


public:
    PtrT<InputStream> stream() const;
    int line() const { return mLine; }
    int column() const { return mCol; }

private:
    InputStream* mStream;
    int mLine, mCol;
};

class Token {
public:
    Token();
    Token(InputStreamRef isr, TokenType type);

    Token(InputStreamRef isr, int num);
    Token(InputStreamRef isr, double num);
    Token(InputStreamRef isr, TokenType type, const QByteArray& text);

    Token(const Token& other);
    Token& operator=(const Token& other);
    ~Token();

public:
    TokenType type() const { return mType; }
    QByteArray text() const { return mText; }

private:
    void assign(const Token& other);

private:
    TokenType mType;
    InputStreamRef mStreamRef;
    QByteArray mText;
    union {
        int mNumberInteger;
        double mNumberDouble;
    };
};

class Tokens
{
public:
    Tokens();
    ~Tokens();

public:
    void init(const QVector<Token>& tokens);

public:
    Token at(int index) const;
    Token current() const { return at(mPos); }
    bool advance() const { return ++mPos != mTokens.count(); }
    Token next() const { advance(); return current(); }
    Token ahead(int count = 1) const { return at(mPos + count); }
    bool require(TokenType type) const { return next().type() == type; }

private:
    QVector< Token > mTokens;
    mutable int mPos;
};

#endif
