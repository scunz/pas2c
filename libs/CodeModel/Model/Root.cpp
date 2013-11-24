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

#include <QFile>

#include "ExportInfo.hpp"
#include "Model/Root.hpp"

namespace Model {

    void Root::setDeclList(const DeclList::Ptr& decls) {
        mDeclList = decls;
    }

    DeclList::Ptr Root::declList() const {
        return mDeclList;
    }

    void Root::setName(const Identifier& identifier) {
        mName = identifier;
    }

    Identifier Root::name() const {
        return mName;
    }

    QDomDocument Root::exportModel() const {

        QDomDocument doc(QLatin1String("PascalModel"));
        ExportInfo ei(doc);

        QDomElement elRoot = doc.createElement(QLatin1String("PascalModel"));
        doc.appendChild(elRoot);

        exportModel(ei, elRoot);

        return doc;
    }

    bool Root::exportModel(const QString& fileName) const {
        QFile fOutput(fileName);
        if (!fOutput.open(QFile::WriteOnly)) {
            return false;
        }

        QDomDocument doc = exportModel();
        fOutput.write(doc.toString().toUtf8());
        return true;
    }

}
