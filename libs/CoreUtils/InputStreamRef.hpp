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

#ifndef PASC_INPUT_STREAM_REF_HPP
#define PASC_INPUT_STREAM_REF_HPP
#pragma once

#include <QDebug>
#include <QString>

// CoreUtils
#include "Errors.hpp"
#include "CoreUtilsAPI.hpp"

class InputStream;

class PASC_CORE_UTILS_API InputStreamRef
{
public:
    InputStreamRef();
    InputStreamRef(const InputStreamRef& other);
    InputStreamRef(InputStream* stream, int line, int column);

public:
    QString fileName() const;
    PtrT<InputStream> stream() const;
    int line() const;
    int column() const;
    QByteArray description() const;

public:
    void emitError(Errors::Ptr errs, const QString& text) const;
    void emitError(Errors::Ptr errs, const char* text) const;

private:
    InputStream* mStream;
    int mLine, mCol;
};

QDebug operator<<(QDebug dbg, const InputStreamRef& ref);

#endif
