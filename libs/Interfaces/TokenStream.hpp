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

#ifndef PASC_IF_TOKEN_STREAM_HPP
#define PASC_IF_TOKEN_STREAM_HPP

#include "PtrBase.hpp"

#include "InterfacesAPI.hpp"
#include "TokenTypes.hpp"

class Token;

class PASC_INTERFACES_API TokenStream : public PtrBase {
public:
    typedef PtrT<TokenStream> Ptr;

public:
    virtual const Token& at(int index) const = 0;
    virtual const Token& current() const = 0;
    virtual bool advance() const = 0;
    virtual const Token& next() const = 0;
    virtual const Token& ahead(int count = 1) const = 0;
    virtual bool isAhead(TokenType type, int count = 1) const = 0;
    virtual bool require(TokenType type) const = 0;

    virtual void dump(const QString& fileName) const = 0;
};

#endif
