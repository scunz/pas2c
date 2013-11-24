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

#ifndef PASC_IDENTIFIER_HPP
#define PASC_IDENTIFIER_HPP

#include <QByteArray>
#include <QVector>
#include <QSet>
#include <QString>

#include "CodeModelAPI.hpp"

namespace Model
{

    class PASC_CODE_MODEL_API Identifier
    {
    public:
        Identifier();
        Identifier(const Identifier& other);
        Identifier(const QByteArray& text);

        Identifier& operator=(const Identifier& other);
        bool operator==(const Identifier& other) const;
        bool operator!=(const Identifier& other) const;
        bool operator<(const Identifier& other) const;

        bool isNull() const;

        QByteArray name() const;
        QString toString() const;

    private:
        QByteArray mName;
    };

    typedef QVector<Identifier> IdentifierList;
    typedef QSet<Identifier>    IdentifierSet;

    PASC_CODE_MODEL_API uint qHash(const Identifier& identifier);

}


#endif
