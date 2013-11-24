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
#include "Model/Code.hpp"

namespace Model {

    ExportInfo::ExportInfo(const QDomDocument& doc)
        : mDoc(doc)
        , mNextId(1)
    {
    }

    QDomElement ExportInfo::newElement(QDomElement& parent, const char* szName) {
        QDomElement el = mDoc.createElement(QLatin1String(szName));
        parent.appendChild(el);
        return el;
    }

    QDomElement ExportInfo::newTextElement(QDomElement& parent, const char* szName, const char* szText) {
        QDomElement el = mDoc.createElement(QLatin1String(szName));
        parent.appendChild(el);

        QDomText txt = mDoc.createTextNode(QLatin1String(szText));
        el.appendChild(txt);

        return el;
    }

    bool ExportInfo::wantExport(const Model::Code* code, QDomElement& parent, QDomElement& me) {
        quint64 id = mExportIds.value(code, 0);
        if (id) {
            newElement(parent, "Ref").setAttribute(QLatin1String("ref"), id);
            return false;
        }
        else {
            me = newElement(parent, code->className());
            me.setAttribute(QLatin1String("id"), mNextId);
            mExportIds[code] = mNextId++;
            return true;
        }
    }

}
