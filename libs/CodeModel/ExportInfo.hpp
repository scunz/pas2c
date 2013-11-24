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

#ifndef PASC_EXPORT_INFO_HPP
#define PASC_EXPORT_INFO_HPP

#include "CodeModelAPI.hpp"
#ifndef PASC_CODE_MODEL_NO_INTERNALS

#include <QHash>
#include <QDomDocument>

namespace Model {

    class Code;

    class ExportInfo
    {
    public:
        ExportInfo(const QDomDocument& doc);

    public:
        QDomElement newElement(QDomElement& parent, const char* szName);
        QDomElement newTextElement(QDomElement& parent, const char* szName, const char* szText);

    public:
        bool wantExport(const Model::Code* code, QDomElement& parent, QDomElement& me);

    private:
        QDomDocument mDoc;
        QHash<const Model::Code*, quint64> mExportIds;
        quint64 mNextId;
    };

}

#endif // PASC_CODE_MODEL_NO_INTERNALS

#endif
