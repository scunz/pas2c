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
#include "Model/Stmt/List.hpp"

namespace Model {

    void StmtList::exportModel(ExportInfo& ei, QDomElement& elParent) const {

        QDomElement elMe;

        if (ei.wantExport(this, elParent, elMe)) {
            foreach (const Stmt::Ptr stmt, mStmts) {
                stmt->exportModel(ei, elMe);
            }
        }
    }

    void StmtList::append(const Stmt::Ptr& stmt) {
        mStmts.append(stmt);
    }

    const char* StmtList::className() const {
        return "Stmt.List";
    }

    Objects StmtList::type() const {
        return oStmtList;
    }

    StmtList::Ptr StmtList::import(const QDomElement& el) {

    }

    int StmtList::count() const {
        return mStmts.count();
    }

    QVector<Stmt::Ptr> StmtList::all() const {
        return mStmts;
    }

    Stmt::Ptr StmtList::at(int idx) const {
        return mStmts.at(idx);
    }

}
