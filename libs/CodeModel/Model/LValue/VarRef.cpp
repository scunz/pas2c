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
#include "Model/LValue/VarRef.hpp"

namespace Model {

    void LValueVarRef::exportModel(ExportInfo& ei, QDomElement& elParent) const {

        QDomElement elMe;
        if (ei.wantExport(this, elParent, elMe)) {
            mVar->exportModel(ei, elMe);
        }
    }

    void LValueVarRef::importModel(const QDomElement& el) {
    }

    const char* LValueVarRef::className() const {
        return "LValue.VarRef";
    }

    Objects LValueVarRef::type() const {
        return oLValueVarRef;
    }

    void LValueVarRef::setVar(const DeclVar::Ptr& decl) {
        mVar = decl;
    }

    DeclVar::Ptr LValueVarRef::var() const {
        return mVar;
    }

}
