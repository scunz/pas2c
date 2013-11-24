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

#ifndef PASC_ERROR_HPP
#define PASC_ERROR_HPP
#pragma once

#include <QDebug>
#include <QString>
#include <QVector>

#include "PtrBase.hpp"

class PASC_CORE_UTILS_API Error : public PtrBase
{
public:
    typedef PtrT<Error> Ptr;
    typedef QVector<Ptr> List;

public:
    Error();
    Error(const QString& text, const QString& fileName, int line = -1, int column = -1);

public:
    QString text() const;
    QString fileName() const;
    int line() const;
    int column() const;

private:
    QString mText, mFileName;
    int mLine, mColumn;
};

PASC_CORE_UTILS_API QDebug operator<<(QDebug dbg, const Error& e);
PASC_CORE_UTILS_API QDebug operator<<(QDebug dbg, const Error::Ptr& e);

#endif
