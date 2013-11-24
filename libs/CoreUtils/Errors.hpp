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

#ifndef PASC_ERRORS_HPP
#define PASC_ERRORS_HPP

#include "Error.hpp"

class PASC_CORE_UTILS_API Errors : public PtrBase
{
public:
    typedef PtrT<Errors> Ptr;

public:
    void emitError(const char* text, const QString& fileName, int line = -1, int column = -1);
    void emitError(const QString& text, const QString& fileName, int line = -1, int column = -1);

public:
    bool hasErrors() const { return mAllErrors.count() > 0; }
    void dumpErrors() const;

public:
    Error::List allErrors() const { return mAllErrors; }

private:
    Error::List mAllErrors;
};

#endif
