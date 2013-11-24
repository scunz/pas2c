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

#ifndef PASC_TOKEN_IMPL_HPP
#define PASC_TOKEN_IMPL_HPP

#include <QByteArray>
#include <QDebug>

// CoreUtils
#include "InputStreamRef.hpp"

// Interfaces
#include "TokenTypes.hpp"
#include "Token.hpp"

class TokenImpl : public Token {
public:
    TokenImpl();
    TokenImpl(const InputStreamRef& isr, TokenType type);

    TokenImpl(const InputStreamRef& isr, quint64 num);
    TokenImpl(const InputStreamRef& isr, double num);
    TokenImpl(const InputStreamRef& isr, TokenType type, const QByteArray& textData);

    TokenImpl(const TokenImpl& other);
    TokenImpl& operator=(const TokenImpl& other);
    ~TokenImpl();

public:
    const InputStreamRef& inputStreamRef() const;
    TokenType type() const;

    QByteArray  text() const;
    QByteArray  identifier() const;
    quint64     number() const;
    double      numberFloat() const;

    QByteArray name() const;
    QByteArray description() const;

private:
    QByteArray textDataRaw() const;
    void assign(const TokenImpl& other);

private:
    TokenType mType;
    InputStreamRef mStreamRef;
    union {
        struct {
            char* text;
            int length;
        } mTextData;
        quint64 mNumberInteger;
        double mNumberDouble;
    };
};

QDebug operator<<(QDebug dbg, const TokenImpl& t);

#endif
