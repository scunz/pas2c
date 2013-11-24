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

#ifndef PASC_UNIT_PARSER_HPP
#define PASC_UNIT_PARSER_HPP
#pragma once

#include "Pascal.hpp"

#include "Model/Root/Unit.hpp"

namespace Parsers {

    class Unit : public Pascal
    {
    public:
        Unit(const ContextStack::Ptr& ctxStack, const Errors::Ptr& errs, const TokenStream::Ptr& stream)
            : Pascal(ctxStack, errs, stream)
        {
            setModel(new Model::RootUnit);
        }

        Model::RootUnit::Ptr model() { return Pascal::model().scast<Model::RootUnit>(); }

    public:
        bool operator()();

    private:
        void importUnit(Model::RootUnit::Ptr unit, bool forInterface);

    private:
        bool parseName();
    };

}

#endif
