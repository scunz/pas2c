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
#include "Identifier.hpp"
#include "Model/Code.hpp"

namespace Model
{

    Code::Code()
    {
    }

    void Code::exportIdentifier(QDomElement& elParent, const Identifier& name) {

        QDomElement el = elParent.ownerDocument().createElement(QLatin1String("Identifier"));
        QDomText txt = elParent.ownerDocument().createTextNode(name.toString());

        el.appendChild(txt);
        elParent.appendChild(el);
    }

}
