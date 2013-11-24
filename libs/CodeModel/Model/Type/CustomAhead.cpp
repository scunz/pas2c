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

#include "Model/Type/CustomAhead.hpp"

namespace Model
{

    void TypeCustomAhead::exportModel(ExportInfo& ei, QDomElement& elParent) const {
        Q_ASSERT(false); // Should be removed already!
    }

    void TypeCustomAhead::importModel(QDomElement& el) {
        Q_ASSERT(false); // Should have never been exported
    }

    const char* TypeCustomAhead::className() const {
        return "Type.Custom.AHead";
    }

    Objects TypeCustomAhead::type() const {
        return oTypeCustomAhead;
    }

    void TypeCustomAhead::setDecl(const Identifier& name) {
        mName = name;
    }

    const Identifier& TypeCustomAhead::name() const {
        return mName;
    }

}
