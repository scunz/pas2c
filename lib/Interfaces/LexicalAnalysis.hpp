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

#ifndef PASC_LEXANAL_HPP
#define PASC_LEXANAL_HPP

// CoreUtils
#include "Errors.hpp"

// Interfaces
#include "Createable.hpp"
#include "TokenStream.hpp"

class PASC_INTERFACES_API LexicalAnalyzer : public Createable
{
public:
    typedef PtrT<LexicalAnalyzer> Ptr;

public:
    virtual void setInput(const QString& fileName) = 0;
    virtual bool analyze() = 0;
    virtual TokenStream::Ptr tokenStream() const = 0;
};

#endif
