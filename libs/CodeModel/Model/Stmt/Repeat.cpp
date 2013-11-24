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
#include "Model/Stmt/Repeat.hpp"

namespace Model {

    void StmtRepeat::exportModel(ExportInfo& ei, QDomElement& elParent) const {
        QDomElement elMe;
        if (ei.wantExport(this, elParent, elMe)) {

            if (mStmtList) {
                mStmtList->exportModel(ei, elMe);
            }

            if (mExpression) {
                mExpression->exportModel(ei, elMe);
            }
        }
    }

    void StmtRepeat::importModel(const QDomElement& el) {

    }

    const char* StmtRepeat::className() const {
        return "Stmt.Repeat";
    }

    Objects StmtRepeat::type() const {
        return oStmtRepeat;
    }

    void StmtRepeat::setStmtList(StmtList::Ptr stmt) {
        mStmtList = stmt;
    }

    void StmtRepeat::setExpr(Expr::Ptr expr) {
        mExpression = expr;
    }

    StmtList::Ptr StmtRepeat::stmtList() const {
        return mStmtList;
    }

    Expr::Ptr StmtRepeat::expr() const {
        return mExpression;
    }

}
