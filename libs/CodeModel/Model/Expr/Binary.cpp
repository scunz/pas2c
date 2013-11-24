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
#include "Model/Expr/Binary.hpp"
#include "Model/Expr/Const.hpp"

namespace Model {

    void ExprBinary::exportModel(ExportInfo& ei, QDomElement& elParent) const {
        QDomElement elMe;
        if (ei.wantExport(this, elParent, elMe)) {

            const char* operName = NULL;
            switch(mOperator) {
            case operAnd:           operName = "AND"; break;
            case operOr:            operName = "OR"; break;
            case operXor:           operName = "XOR"; break;
            case operLess:          operName = "LES"; break;
            case operLessEqual:     operName = "LEQ"; break;
            case operGreater:       operName = "GRT"; break;
            case operGreaterEqual:  operName = "GEQ"; break;
            case operEqual:         operName = "EQU"; break;
            case operInequal:       operName = "NEQ"; break;
            case operIn:            operName = "IN"; break;
            case operAdd:           operName = "ADD"; break;
            case operMinus:         operName = "SUB"; break;
            case operMult:          operName = "MUL"; break;
            case operDivision:      operName = "DIV"; break;
            case operIntDiv:        operName = "IDIV"; break;
            case operIntMod:        operName = "IMOD"; break;
            case operShiftLeft:     operName = "SHL"; break;
            case operShiftRight:    operName = "SHR"; break;
            }

            if (operName) {
                ei.newTextElement(elMe, "Expr.Binary.Oper", operName);
            }

            if (mLOperand) {
                QDomElement elLeft = ei.newElement(elMe, "Expr.Binary.Left");
                mLOperand->exportModel(ei, elLeft);
            }

            if (mROperand) {
                QDomElement elRight = ei.newElement(elMe, "Expr.Binary.Right");
                mROperand->exportModel(ei, elRight);
            }
        }
    }

    void ExprBinary::importModel(const QDomElement& el) {
        Q_ASSERT(el.tagName() == QLatin1String("ExprBinary"));

    }

    bool ExprBinary::isConst() const {
        return
            mLOperand && mLOperand->isConst() &&
            mROperand && mROperand->isConst();
    }

    const char* ExprBinary::className() const {
        return "Expr.Binary";
    }

    Objects ExprBinary::type() const {
        return oExprBinary;
    }

    ExprConst::Ptr ExprBinary::constEval() const {
        Q_ASSERT(isConst());
        return NULL;
    }

    BinaryOperator ExprBinary::binaryOperator() const {
        return mOperator;
    }

    void ExprBinary::setBinaryOperator(const BinaryOperator& op) {
        mOperator = op;
    }

    Expr::Ptr ExprBinary::leftOperand() const {
        return mLOperand;
    }

    Expr::Ptr ExprBinary::rightOperand() const {
        return mROperand;
    }

    void ExprBinary::setLeftOperand(const Expr::Ptr& o) {
        mLOperand = o;
    }

    void ExprBinary::setRightOperand(const Expr::Ptr& o) {
        mROperand = o;
    }

}
