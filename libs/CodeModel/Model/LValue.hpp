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

#ifndef PASC_MODEL_LVALUE_HPP
#define PASC_MODEL_LVALUE_HPP
#pragma once

#include "Model/Code.hpp"

namespace Model {

    class PASC_CODE_MODEL_API LValue : public Code
    {
    public:
        typedef PtrT<LValue> Ptr;

    public:
        virtual void importModel(const QDomElement& el) = 0;

    public:
        static LValue::Ptr import(const QDomElement& el);
    };

}

#endif
