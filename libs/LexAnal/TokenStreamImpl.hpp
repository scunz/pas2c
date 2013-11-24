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

#ifndef PASC_TOKEN_STREAM_HPP
#define PASC_TOKEN_STREAM_HPP
#pragma once

#include <QVector>
#include <QHash>

// CoreUtils
#include "InputStream.hpp"
#include "InputStreamCollection.hpp"

// Interfaces
#include "TokenStream.hpp"

// LexAnal
#include "TokenImpl.hpp"

class TokenStreamImpl : public TokenStream
{
public:
    typedef PtrT<TokenStreamImpl> Ptr;

public:
    TokenStreamImpl();
    ~TokenStreamImpl();

public:
    void appendTokens(const QVector<TokenImpl> &tokens);
    void appendToken(const TokenImpl& token);
    void appendInputStream(InputStream::Ptr stream);

public:
    const Token& at(int index) const;
    const Token& current() const;
    bool advance() const;
    const Token& next() const;
    const Token& ahead(int count = 1) const;
    bool isAhead(TokenType type, int count = 1) const;
    bool require(TokenType type) const;
    void dump(const QString& fileName) const;

public:
    bool finalize();

private:
    InputStreamCollection   mInputStreams;
    QVector<TokenImpl>      mTokens;
    mutable int             mPos;
};


#endif
