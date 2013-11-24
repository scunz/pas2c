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

#ifndef PASC_DECL_LIST_PARSER_HPP
#define PASC_DECL_LIST_PARSER_HPP
#pragma once

#include "Pascal.hpp"

#include "Model/Decl/List.hpp"
#include "Model/Decl/Routine.hpp"

namespace Parsers {

    class DeclList : public Base
    {
        DECL_PARSER_INIT_WITH_MODEL(DeclList,Base,DeclList)

    public:
        enum Option {
            ForInterface            = 1UL << 0,
            ForImplementation       = 1UL << 1,
        };
        typedef QFlags<Option> Options;

    public:
        void setOptions(Options opts) { mOpts = opts; }
        Options options() const { return mOpts; }
        bool testOption(Option opt) { return mOpts.testFlag(opt); }

        bool operator()();

    private:
        bool parseVar();
        bool parseType();
        bool parseConst();
        bool parseProc();
        bool parseFunc();

        bool parseBody(Model::DeclRoutine::Ptr routine);

    private:
        Options mOpts;
    };

}

#endif
