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

#ifndef PASC_PASCAL_PARSER_HPP
#define PASC_PASCAL_PARSER_HPP

#include "ParsersAPI.hpp"
#include "Base.hpp"
#include "Project.hpp"

#include "Model/Root.hpp"
#include "Model/Root/Unit.hpp"

namespace Parsers {

    class PASC_PARSERS_API Pascal : public Base
    {
    protected:
        Pascal(const ContextStack::Ptr& ctxStack, const Errors::Ptr& errs, const TokenStream::Ptr& stream)
            : Base(ctxStack, errs, stream)
        {}

    public:
        void setProject(Project::Ptr project);
        Project::Ptr project() const;

    public:
        static Pascal* create(const ContextStack::Ptr& ctxStack,
                              Errors::Ptr errs,
                              const TokenStream::Ptr& stream);

    public:
        Model::Root::Ptr model() { return Base::model().scast<Model::Root>(); }

    public:
        virtual bool operator()() = 0;

    protected:
        bool parseUses(bool forInterface);

        virtual void importUnit(Model::RootUnit::Ptr unit, bool forInterface) = 0;

    private:
        Project::Ptr mProject;
    };

}

#endif
