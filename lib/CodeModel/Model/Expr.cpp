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
#include "Model/Expr.hpp"
#include "Model/Expr/Const.hpp"
#include "Model/Expr/Unary.hpp"
#include "Model/Expr/Binary.hpp"

namespace Model
{

    ExprConst::Ptr Expr::constEval() const {
        Q_ASSERT(isConst());
        return NULL;
    }

    Expr::Ptr Expr::import(const QDomElement& el)
    {
        Expr* expr = NULL;
        QString tag = el.tagName();

        if (tag == QLatin1String("Unary")) {
            expr = new ExprUnary;
        }
        else if (tag == QLatin1String("Binary")) {
            expr = new ExprBinary;
        }

        if (expr) {
            expr->import(el);
        }

        return expr;
    }

}
