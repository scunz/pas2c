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

#ifndef PASC_LVALUE_PARSER_HPP
#define PASC_LVALUE_PARSER_HPP
#pragma once

#include "Base.hpp"

#include "Model/Decl.hpp"
#include "Model/Decl/Var.hpp"
#include "Model/LValue.hpp"

namespace Parsers {

    class LValue : public Base
    {
    public:
        enum Option {
            None                        = 0,
            RequireVariable             = 1UL << 0,
            AllowProcedureCall          = 1UL << 1,
            AllowFunctionCall           = 1UL << 2,
        };
        typedef QFlags<Option> Options;

    public:
        DECL_PARSER_INIT_WITHOUT_MODEL(LValue, Base, LValue)

        void setOptions(Options opts) { mOpts = opts; }
        Options options() const { return mOpts; }
        bool testOption(Option opt) { return mOpts.testFlag(opt); }

        bool operator()();

    private:
        bool parseVarRef(const Model::DeclVar::Ptr& decl);
        bool parseTypeCast(const Model::Type::Ptr& type);
        bool parseRightAssoced(const Model::LValue::Ptr& lv);
        bool parsePointerDeref(const Model::LValue::Ptr& lv);
        bool parseRecordDeref(const Model::LValue::Ptr& lv);
        bool parseArraySubscript(const Model::LValue::Ptr& lv);

    private:
        Options mOpts;
    };

}

#endif
