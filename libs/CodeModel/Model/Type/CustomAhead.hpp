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

#ifndef PASC_MODEL_TYPE_CUSTOM_AHEAD_HPP
#define PASC_MODEL_TYPE_CUSTOM_AHEAD_HPP

#include "Model/Type.hpp"

namespace Model
{

    class PASC_CODE_MODEL_API TypeCustomAhead : public Type
    {
    public:
        typedef PtrT<TypeCustomAhead> Ptr;

    public:
        void exportModel(ExportInfo& ei, QDomElement& elParent) const;
        void importModel(QDomElement& el);

    public:
        void setDecl(const Identifier& name);
        const Identifier& name() const;

    public:
        const char* className() const;
        Objects type() const;

    private:
        Identifier mName;
    };

}

#endif
