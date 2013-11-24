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
#include "Model/Type/File.hpp"
#include "Model/Type/Custom.hpp"
#include "Model/Type/CustomAhead.hpp"

namespace Model
{

    void TypeFileOf::exportModel(ExportInfo& ei, QDomElement& elParent) const {

        QDomElement elMe;
        if (ei.wantExport(this, elParent, elMe)) {
            base()->exportModel(ei, elMe);
        }
    }

    void TypeFileOf::importModel(QDomElement& el) {
        Q_ASSERT(el.tagName() == QLatin1String("FileOf"));
        setBase(Type::import(el.firstChildElement()));
    }

    const char* TypeFileOf::className() const {
        return "Type.FileOf";
    }

    Objects TypeFileOf::type() const {
        return oTypeFileOf;
    }

    void TypeFileOf::replaceAheadType(const Identifier& name, const DeclType::Ptr& realType) {
        if (base()->type() == oTypeCustomAhead) {
            TypeCustomAhead::Ptr tca = base().scast<TypeCustomAhead>();
            if (tca->name() == name) {
                TypeCustom* tc = new TypeCustom;
                tc->setDecl(realType);
                setBase(tc);
            }
        }
        else {
            base()->replaceAheadType(name, realType);
        }
    }

}
