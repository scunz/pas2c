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

#ifndef PASC_TYPE_PARSER_HPP
#define PASC_TYPE_PARSER_HPP
#pragma once

#include "Base.hpp"

#include "Model/Type.hpp"

namespace Parsers {

    class Type : public Base
    {
    public:
        enum Option {
            tpoNone = 0,
            tpoAllowAheadReference      = 1UL << 0,
        };
        typedef QFlags<Option> Options;

    public:
        DECL_PARSER_INIT_WITHOUT_MODEL(Type, Base, Type)

        void setOptions(Options opts) { mOpts = opts; }
        Options options() const { return mOpts; }
        bool testOption(Option opt) { return mOpts.testFlag(opt); }

        bool operator()();

        bool hasAhead() const { return !mAheadName.isNull(); }
        const Model::Identifier& aheadName() const { return mAheadName; }

    private:
        bool parseRecord();
        bool parseRecordFieldList();

    private:
        Options mOpts;
        Model::Identifier mAheadName;
    };

}

#endif
