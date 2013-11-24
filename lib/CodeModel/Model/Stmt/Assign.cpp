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
#include "Model/Stmt/Assign.hpp"

namespace Model {

    void StmtAssign::exportModel(ExportInfo& ei, QDomElement& elParent) const {

        QDomElement elMe;
        if (ei.wantExport(this, elParent, elMe)) {

            if (mLValue) {
                mLValue->exportModel(ei, elMe);
            }

            if (mExpression) {
                mExpression->exportModel(ei, elMe);
            }
        }
    }

    void StmtAssign::importModel(const QDomElement& el) {

    }

    const char* StmtAssign::className() const {
        return "Stmt.Assign";
    }

    Objects StmtAssign::type() const {
        return oStmtAssign;
    }

    void StmtAssign::setLValue(LValue::Ptr lvalue) {
        mLValue = lvalue;
    }

    void StmtAssign::setExpr(Expr::Ptr expr) {
        mExpression = expr;
    }

    LValue::Ptr StmtAssign::lValue() const {
        return mLValue;
    }

    Expr::Ptr StmtAssign::expr() const {
        return mExpression;
    }

}

