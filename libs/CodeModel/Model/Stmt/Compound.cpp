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
#include "Model/Stmt/Compound.hpp"

namespace Model {

    void StmtCompound::exportModel(ExportInfo& ei, QDomElement& elParent) const {

        QDomElement elMe;
        if (ei.wantExport(this, elParent, elMe)) {

            if (mStatements) {
                mStatements->exportModel(ei, elMe);
            }
        }
    }

    void StmtCompound::importModel(const QDomElement& el) {

    }

    const char* StmtCompound::className() const {
        return "Stmt.Compound";
    }

    Objects StmtCompound::type() const {
        return oStmtCompound;
    }

    void StmtCompound::setStatements(StmtList::Ptr stmts) {
        mStatements = stmts;
    }

    StmtList::Ptr StmtCompound::statements() const {
        return mStatements;
    }

}
