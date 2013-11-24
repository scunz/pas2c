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

#include "ExportInfo.hpp"
#include "Model/Type/Stock.hpp"

namespace Model
{

    void Type::replaceAheadType(const Identifier& name, const PtrT<DeclType>& realType) {
        Q_UNUSED(name);
        Q_UNUSED(realType);
    }

    Type::Ptr Type::import(const QDomElement& el) {
        Type* t = NULL;

        QString name = el.tagName();

        if (name == QLatin1String("Stock")) {
            t = new TypeStock;
        }

        if (t) {
            t->import(el);
        }

        return t;
    }

}
