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
#include "Model/Type/Record.hpp"

namespace Model
{

    void TypeRecord::exportModel(ExportInfo& ei, QDomElement &elParent) const {
        QDomElement elMe;

        if (ei.wantExport(this, elParent, elMe)) {

            foreach (const TypeRecordField::Ptr& f, mFields) {
                f->exportModel(ei, elMe);
            }

        }
    }

    void TypeRecord::importModel(QDomElement &el) {

    }

    void TypeRecord::addField(const Identifier& name, const Type::Ptr& type) {
        TypeRecordField::Ptr rf = new TypeRecordField;
        rf->setName(name);
        rf->setDataType(type);
        mFields.append(rf);
        mFieldsByName.insert(name, rf);
    }

    const char* TypeRecord::className() const {
        return "Type.Record";
    }

    Objects TypeRecord::type() const {
        return oTypeRecordOf;
    }

}
