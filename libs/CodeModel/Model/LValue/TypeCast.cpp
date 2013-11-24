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
#include "Model/LValue/TypeCast.hpp"

namespace Model {

    void LValueTypeCast::exportModel(ExportInfo& ei, QDomElement& elParent) const {
        QDomElement elMe;
        if (ei.wantExport(this, elParent, elMe)) {

            if (mType) {
                mType->exportModel(ei, elMe);
            }

            if (mSource) {
                mSource->exportModel(ei, elMe);
            }
        }
    }

    void LValueTypeCast::importModel(const QDomElement& el) {
    }

    const char* LValueTypeCast::className() const {
        return "LValue.TypeCast";
    }

    Objects LValueTypeCast::type() const {
        return oLValueTypeCast;
    }

    void LValueTypeCast::setDataType(Type::Ptr type) {
        mType = type;
    }

    void LValueTypeCast::setSource(LValue::Ptr source) {
        mSource = source;
    }

    LValue::Ptr LValueTypeCast::source() const {
        return mSource;
    }

    Type::Ptr LValueTypeCast::dataType() const {
        return mType;
    }

}
