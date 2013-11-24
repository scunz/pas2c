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
#include "Model/Stmt/If.hpp"

namespace Model {

    void StmtIf::exportModel(ExportInfo& ei, QDomElement& elParent) const {
        QDomElement elMe;
        if (ei.wantExport(this, elParent, elMe)) {
            if (mExpression) {
                mExpression->exportModel(ei, elMe);
            }

            if (mTrueStatement) {
                QDomElement elTrue = ei.newElement(elMe, "Stmt.If.On.True");
                mTrueStatement->exportModel(ei, elTrue);
            }

            if (mFalseStatement) {
                QDomElement elFalse = ei.newElement(elMe, "Stmt.If.On.False");
                mFalseStatement->exportModel(ei, elFalse);
            }
        }
    }

    void StmtIf::importModel(const QDomElement& el) {

    }

    const char* StmtIf::className() const {
        return "Stmt.If";
    }

    Objects StmtIf::type() const {
        return oStmtIf;
    }

    void StmtIf::setTrueStatement(Stmt::Ptr stmt) {
        mTrueStatement = stmt;
    }

    void StmtIf::setFalseStatement(Stmt::Ptr stmt) {
        mFalseStatement = stmt;
    }

    void StmtIf::setExpr(Expr::Ptr expr) {
        mExpression = expr;
    }

    Stmt::Ptr StmtIf::trueStatement() const {
        return mTrueStatement;
    }

    Stmt::Ptr StmtIf::falseStatement() const {
        return mFalseStatement;
    }

    Expr::Ptr StmtIf::expr() const {
        return mExpression;
    }

}
