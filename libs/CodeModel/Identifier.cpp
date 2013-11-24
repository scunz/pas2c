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

#include "Identifier.hpp"

namespace Model {

    Identifier::Identifier()
    {}

    Identifier::Identifier(const Identifier& other)
        : mName(other.mName)
    {}

    Identifier::Identifier(const QByteArray& text)
        : mName(text)
    {}

    Identifier& Identifier::operator=(const Identifier& other)
    {
        mName = other.mName;
        return *this;
    }

    bool Identifier::operator==(const Identifier& other) const
    {
        return mName.toLower() == other.name().toLower();
    }

    bool Identifier::operator!=(const Identifier& other) const
    {
        return mName.toLower() != other.name().toLower();
    }

    bool Identifier::operator<(const Identifier& other) const
    {
        return mName.toLower() < other.name().toLower();
    }

    bool Identifier::isNull() const {
        return mName.isEmpty();
    }

    QByteArray Identifier::name() const {
        return mName;
    }

    QString Identifier::toString() const {
        return QString::fromUtf8(mName.constData());
    }

    uint qHash(const Identifier& identifier) {
        return qHash(identifier.name().toLower());
    }

}

