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

#include "ExportInfo.hpp"
#include "Model/Root/Unit.hpp"

namespace Model
{

    void RootUnit::exportModel(ExportInfo& ei, QDomElement& elParent) const {

        QDomElement elMe;
        if (ei.wantExport(this, elParent, elMe)) {

            exportIdentifier(elMe, name());

            if (declList() && declList()->count()) {
                QDomElement elInterface = ei.newElement(elMe, "Root.Unit.Interface");
                declList()->exportModel(ei, elInterface);
            }

            if (implList() && implList()->count()) {
                QDomElement elImplementation = ei.newElement(elMe, "Root.Unit.Implementation");
                implList()->exportModel(ei, elImplementation);
            }
        }
    }

    RootUnit::Ptr RootUnit::importModel(QDomElement& el) {

    }

    const char* RootUnit::className() const {
        return "Root.Unit";
    }

    Objects RootUnit::type() const {
        return oUnit;
    }

}
