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

#ifndef PASC_MODEL_DECL_LIST_HPP
#define PASC_MODEL_DECL_LIST_HPP
#pragma once

#include <QHash>
#include <QVector>

#include "Model/Decl.hpp"
#include "Model/Decl/Type.hpp"

namespace Model
{

    class PASC_CODE_MODEL_API DeclList: public Code
    {
    public:
        typedef PtrT<DeclList> Ptr;

    public:
        void exportModel(ExportInfo& ei, QDomElement& elParent) const;

    public:
        int count() const;
        QVector<Decl::Ptr> all() const;
        Decl::Ptr at(int idx) const;
        void append(const Decl::Ptr& decl);

        Decl::Ptr get(const Identifier& id) const;
        bool has(const Identifier& id) const;
        bool has(const Identifier& id, Objects type) const;
        Objects typeOf(const Identifier& id) const;

    public:
        void replaceAheadTypes(const Identifier& name, const DeclType::Ptr& realType);

    public:
        static DeclList::Ptr import(const QDomElement& el);

    public:
        const char* className() const;
        Objects type() const;

    private:
        QVector<Decl::Ptr> mDecls;
        QHash<Identifier, Decl::Ptr> mDeclsById;
    };

}

#endif
