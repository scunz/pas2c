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

#ifndef PASC_MODEL_STMT_IF_HPP
#define PASC_MODEL_STMT_IF_HPP
#pragma once

#include "Model/Stmt.hpp"
#include "Model/Expr.hpp"

namespace Model {

    class PASC_CODE_MODEL_API StmtIf : public Stmt
    {
    public:
        typedef PtrT<StmtIf> Ptr;

    public:
        void exportModel(ExportInfo& ei, QDomElement& elParent) const;
        void importModel(const QDomElement& el);

    public:
        void setTrueStatement(Stmt::Ptr stmt);
        void setFalseStatement(Stmt::Ptr stmt);
        void setExpr(Expr::Ptr expr);

        Stmt::Ptr trueStatement() const;
        Stmt::Ptr falseStatement() const;
        Expr::Ptr expr() const;

    public:
        const char* className() const;
        Objects type() const;

    private:
        Stmt::Ptr mTrueStatement, mFalseStatement;
        Expr::Ptr mExpression;
    };

}

#endif
