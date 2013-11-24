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

#include "Errors.hpp"

void Errors::emitError(const QString &text, const QString &fileName, int line, int column) {
    Error::Ptr e = new Error(text, fileName, line, column);
    mAllErrors.append(e);
}

void Errors::emitError(const char* text, const QString &fileName, int line, int column) {
    emitError(QString::fromUtf8(text), fileName, line, column);
}

void Errors::dumpErrors() const {
    foreach (const Error::Ptr& e, mAllErrors) {
        qDebug() << *e;
    }
}
