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
#include "Model/Expr/Const/Number.hpp"

namespace Model {

    ExprConstNumber::ExprConstNumber(quint64 number)
        : mNumber(number)
    {
    }

    void ExprConstNumber::exportModel(ExportInfo& ei, QDomElement& elParent) const {
        QDomElement elMe;
        if (ei.wantExport(this, elParent, elMe)) {
            ei.newTextElement(elMe, "Expr.Const.Number.Value",
                              QByteArray::number(mNumber).constData());
        }
    }

    void ExprConstNumber::importModel(const QDomElement &el) {
    }

    const char* ExprConstNumber::className() const {
        return "Expr.Const.Number";
    }

    Objects ExprConstNumber::type() const {
        return oExprConstNumber;
    }

    quint64 ExprConstNumber::value() const {
        return mNumber;
    }

}
