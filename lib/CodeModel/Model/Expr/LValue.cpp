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
#include "Model/Expr/LValue.hpp"

namespace Model {

    void ExprLValue::exportModel(ExportInfo& ei, QDomElement& elParent) const {

    }

    void ExprLValue::importModel(const QDomElement &el) {
    }

    const char* ExprLValue::className() const {
        return "Expr.LValue";
    }

    Objects ExprLValue::type() const {
        return oExprLValue;
    }

    bool ExprLValue::isConst() const {
        return false;
    }

    void ExprLValue::setLValue(const LValue::Ptr lv) {
        mLValue = lv;
    }

    LValue::Ptr ExprLValue::lValue() const {
        return mLValue;
    }

}
