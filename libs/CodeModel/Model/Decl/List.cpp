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
#include "Model/Decl/List.hpp"

namespace Model
{

    void DeclList::exportModel(ExportInfo& ei, QDomElement& elParent) const {

        QDomElement elMe;
        if (ei.wantExport(this, elParent, elMe)) {
            foreach (const Decl::Ptr& decl, mDecls) {
                decl->exportModel(ei, elMe);
            }
        }
    }

    Objects DeclList::type() const {
        return oDeclList;
    }

    DeclList::Ptr DeclList::import(const QDomElement& el) {

        DeclList::Ptr l = new DeclList;

        Q_ASSERT(el.tagName() == QLatin1String("DeclList"));

        QDomNodeList nl = el.childNodes();
        for (int i = 0; i < nl.count(); i++) {
            QDomElement elChild = nl.at(i).toElement();
            if (elChild.isNull()) {
                continue;
            }

            Decl::Ptr p = Decl::import(elChild);
            if (!p) {
                return DeclList::Ptr();
            }
            l->append(p);
        }

        return l;
    }

    int DeclList::count() const {
        return mDecls.count();
    }

    QVector<Decl::Ptr> DeclList::all() const {
        return mDecls;
    }

    Decl::Ptr DeclList::at(int idx) const {
        return mDecls.at(idx);
    }

    void DeclList::append(const Decl::Ptr& decl)
    {
        Q_ASSERT(!has(decl->name()));

        mDecls.append(decl);
        mDeclsById.insert(decl->name(), decl);
    }

    bool DeclList::has(const Identifier &id) const {
        return mDeclsById.contains(id);
    }

    Decl::Ptr DeclList::get(const Identifier &id) const {
        return mDeclsById.value(id, Decl::Ptr());
    }

    bool DeclList::has(const Identifier& id, Objects type) const {
        return typeOf(id) == type;
    }

    Objects DeclList::typeOf(const Identifier& id) const {
        Decl::Ptr decl = get(id);
        if (!decl) {
            return oInvalid;
        }
        return decl->type();
    }

    void DeclList::replaceAheadTypes(const Identifier& name, const DeclType::Ptr& realType) {
        foreach (Decl::Ptr decl, mDecls) {
            if (decl->type() == oDeclType) {
                DeclType::Ptr declType = decl.scast<DeclType>();
                declType->dataType()->replaceAheadType(name, realType);
            }
        }
    }

    const char* DeclList::className() const {
        return "Decl.List";
    }

}

