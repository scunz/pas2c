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

#include <QDebug>
#include <QByteArray>
#include <QStringBuilder>

#include "InputStreamRef.hpp"
#include "InputStream.hpp"

InputStreamRef::InputStreamRef()
    : mStream(NULL)
    , mLine(-1)
    , mCol(-1)
{
}

InputStreamRef::InputStreamRef(const InputStreamRef& other)
    : mStream(other.mStream)
    , mLine(other.mLine)
    , mCol(other.mCol)
{
}

InputStreamRef::InputStreamRef(InputStream* stream, int line, int column)
    : mStream(stream)
    , mLine(line)
    , mCol(column)
{
}

QString InputStreamRef::fileName() const
{
    return mStream ? mStream->fileName() : QString();
}

QByteArray InputStreamRef::description() const {

    QByteArray a;
    a = fileName().toUtf8();

    if (mLine) {
        a = a % ':' % QByteArray::number(mLine);
        if (mCol) {
            a = a % ':' % QByteArray::number(mCol);
        }
    }

    return a;
}

void InputStreamRef::emitError(Errors::Ptr errs, const QString& text) const {
    Q_ASSERT(errs);
    errs->emitError(text, fileName(), mLine, mCol);
}

void InputStreamRef::emitError(Errors::Ptr errs, const char* text) const {
    Q_ASSERT(errs);
    errs->emitError(QString::fromUtf8(text), fileName(), mLine, mCol);
}

int InputStreamRef::line() const {
    return mLine;
}

int InputStreamRef::column() const {
    return mCol;
}

PtrT<InputStream> InputStreamRef::stream() const
{
    return mStream;
}

QDebug operator<<(QDebug dbg, const InputStreamRef& ref) {
    return dbg << ref.description().constData();
}
