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
#include "Model/Expr/Unary.hpp"
#include "Model/Expr/Const.hpp"

namespace Model
{

    void ExprUnary::exportModel(ExportInfo& ei, QDomElement& elParent) const {
        QDomElement elMe;
        if (ei.wantExport(this, elParent, elMe)) {

            const char* operName = NULL;
            switch(mOperator) {
            case operNot:           operName = "NOT"; break;
            case operAddrOf:        operName = "ADDR.OF"; break;
            case operKeepSign:      operName = "KEEP.SIGN"; break;
            case operToggleSign:    operName = "CHG.SIGN"; break;
            }

            if (operName) {
                ei.newTextElement(elMe, "Expr.Unary.Oper", operName);
            }

            if (mOperand) {
                QDomElement elOperand = ei.newElement(elMe, "Expr.Unary.Operand");
                mOperand->exportModel(ei, elOperand);
            }
        }
    }

    void ExprUnary::importModel(const QDomElement& el) {
        Q_ASSERT(el.tagName() == QLatin1String("UnaryExpr"));

    }

    bool ExprUnary::isConst() const {
        return mOperand && mOperand->isConst();
    }

    const char* ExprUnary::className() const {
        return "Expr.Unary";
    }

    Objects ExprUnary::type() const {
        return oExprUnary;
    }

    ExprConst::Ptr ExprUnary::constEval() const {
        Q_ASSERT(false);
        Q_ASSERT(isConst());
        return NULL;
    }

    UnaryOperator ExprUnary::unaryOperator() const {
        return mOperator;
    }

    void ExprUnary::setUnaryOperator(const UnaryOperator& op) {
        mOperator = op;
    }

    Expr::Ptr ExprUnary::operand() const {
        return mOperand;
    }

    void ExprUnary::setOperand(const Expr::Ptr& o) {
        mOperand = o;
    }

}
