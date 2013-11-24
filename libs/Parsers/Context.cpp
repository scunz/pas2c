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

#include "Context.hpp"

namespace Parsers {

    ContextStack::ContextStack() {
        // create an outer most context, so there is always one available
        push(new Context);
    }

    ContextStack::~ContextStack() {
        pull(); // Pull the outer most context
        Q_ASSERT(mStack.count() == 0);
    }

    void ContextStack::push(Context::Ptr pcs) {
        mStack.append(pcs);
    }

    void ContextStack::pull() {
        mStack.pop_back();
    }

    int ContextStack::size() const {
        return mStack.count();
    }

    Context::Ptr ContextStack::top() const {
        return mStack.last();
    }

    Context::Ptr Context::inherited(Context::Ptr inherit) {
        Context* pc = new Context;
        pc->mDeclsInScope = inherit->mDeclsInScope;
        return pc;
    }

    Model::Decl::Ptr Context::lookup(const Model::Identifier& identifier) const {

        int i = mDeclsInScope.count();
        while (i-- > 0) {
            Model::DeclList::Ptr list = mDeclsInScope.at(i);
            if (list->has(identifier)) {
                return list->get(identifier);
            }
        }

        return NULL;
    }

    Model::Decl::Ptr Context::lookupRestricted(const Model::Identifier& identifier,
                                                     const QSet<Model::Objects>& types) const {

        int i = mDeclsInScope.count();
        while (i-- > 0) {
            Model::DeclList::Ptr list = mDeclsInScope.at(i);
            if (list->has(identifier)) {
                Model::Decl::Ptr decl = list->get(identifier);
                if (types.contains(decl->type())) {
                    return decl;
                }
            }
        }

        return NULL;
    }

}
