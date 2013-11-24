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

#ifndef PASC_TOKEN_HPP
#define PASC_TOKEN_HPP

class QByteArray;
class InputStreamRef;

#include "TokenTypes.hpp"

class Token {
public:
    virtual const InputStreamRef& inputStreamRef() const = 0;
    virtual TokenType type() const = 0;

    virtual QByteArray  text() const = 0;
    virtual QByteArray  identifier() const = 0;
    virtual quint64     number() const = 0;
    virtual double      numberFloat() const = 0;
};

#endif
