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

#ifndef PASC_STMT_PARSER_HPP
#define PASC_STMT_PARSER_HPP
#pragma once

#include "Base.hpp"

#include "Model/Stmt.hpp"
#include "Model/LValue.hpp"

namespace Parsers {

    class Stmt : public Base
    {
    public:
        enum Option {
            None = 0
        };
        typedef QFlags<Option> Options;

    public:
        DECL_PARSER_INIT_WITHOUT_MODEL(Stmt, Base, Stmt)

        void setOptions(Options opts) { mOpts = opts; }
        Options options() const { return mOpts; }
        bool testOption(Option opt) { return mOpts.testFlag(opt); }

        bool operator()();

    private:
        bool parseCompound();
        bool parseIf();
        bool parseFor();
        bool parseWhile();
        bool parseRepeat();
        bool parseWith();
        bool parseCase();

        bool parseAssignment(Model::LValue::Ptr lvalue);
        bool parseAssignment2(Model::LValue::Ptr lvalue, Model::Stmt::Ptr& stmt);
        bool parseProcCall(Model::LValue::Ptr lvalue);

    private:
        Options mOpts;
    };

}

#endif
