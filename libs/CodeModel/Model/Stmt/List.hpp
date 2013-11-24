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

#ifndef PASC_MODEL_STMT_LIST_HPP
#define PASC_MODEL_STMT_LIST_HPP
#pragma once

#include <QHash>
#include <QVector>

#include "Model/Stmt.hpp"

namespace Model
{

    class PASC_CODE_MODEL_API StmtList : public Code
    {
    public:
        typedef PtrT<StmtList> Ptr;

    public:
        void exportModel(ExportInfo& ei, QDomElement& elParent) const;

    public:
        int count() const;
        QVector<Stmt::Ptr> all() const;
        Stmt::Ptr at(int idx) const;
        void append(const Stmt::Ptr& stmt);

    public:
        static StmtList::Ptr import(const QDomElement& el);

    public:
        const char* className() const;
        Objects type() const;

    private:
        QVector<Stmt::Ptr> mStmts;
    };

}

#endif
