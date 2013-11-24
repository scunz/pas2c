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

#include "Error.hpp"

Error::Error()
    : mText(QString::fromLatin1("No error"))
    , mFileName(QString::fromLatin1("Anywhere"))
    , mLine(-1), mColumn(-1)
{
}

Error::Error(const QString& text, const QString& fileName, int line, int column)
    : mText(text)
    , mFileName(fileName)
    , mLine(line)
    , mColumn(column)
{
}

QString Error::text() const {
    return mText;
}

QString Error::fileName() const {
    return mFileName;
}

int Error::line() const {
    return mLine;
}

int Error::column() const {
    return mColumn;
}

QDebug operator<<(QDebug dbg, const Error& err) {
    if (err.line() != -1) {
        if (err.column() != -1) {
            dbg.nospace() << qPrintable(err.fileName()) << ":"
                          << err.line() << ":" << err.column() << ": "
                          << qPrintable(err.text());
        }
        else {
            dbg.nospace() << qPrintable(err.fileName()) << ":"
                          << err.line() << ": "
                          << qPrintable(err.text());
        }
    }
    else {
        dbg.nospace() << qPrintable(err.fileName()) << ": " << qPrintable(err.text());
    }

    return dbg.space();
}

QDebug operator<<(QDebug dbg, const Error::Ptr& e) {
    return dbg << *e;
}
