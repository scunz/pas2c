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

#ifndef PASC_PARSER_CONTEXT_HPP
#define PASC_PARSER_CONTEXT_HPP

#include <QSet>
#include <QVector>

// CoreUtils
#include "PtrBase.hpp"

// CodeModel
#include "Identifier.hpp"
#include "Model/Decl.hpp"
#include "Model/Decl/List.hpp"

#include "ParsersAPI.hpp"

namespace Parsers {

    class PASC_PARSERS_API Context : public PtrBase
    {
    public:
        typedef PtrT<Context> Ptr;
    public:
        Context() {} // create an EMPTY ParserContext

    public:
         // Inherit all contexts from another
        static Context::Ptr inherited(Context::Ptr inherit);

    public:
        Model::Decl::Ptr lookup(const Model::Identifier& identifier) const;
        Model::Decl::Ptr lookupRestricted(const Model::Identifier& identifier,
                                          const QSet<Model::Objects>& types) const;

    public:
        void append(Model::DeclList::Ptr list) { mDeclsInScope.append(list); }

    private:
        QVector<Model::DeclList::Ptr> mDeclsInScope;
    };

    class PASC_PARSERS_API ContextStack : public PtrBase
    {
    public:
        typedef PtrT<ContextStack> Ptr;
    public:
        ContextStack();
        ~ContextStack();

    public:
        void push(Context::Ptr pcs);
        void pull();
        int size() const;

        Context::Ptr top() const;

    private:
        QVector<Context::Ptr> mStack;
    };

}

#endif
