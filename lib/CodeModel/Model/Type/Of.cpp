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

#include "Model/Type/Of.hpp"

namespace Model {

    TypeOf::TypeOf(const Type::Ptr& baseType)
        : mBase(baseType)
    {
    }

    Type::Ptr TypeOf::base() const {
        return mBase;
    }

    void TypeOf::setBase(const Type::Ptr& base) {
        mBase = base;
    }

}
