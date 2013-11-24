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

#include "StmtList.hpp"
#include "Stmt.hpp"

#include "Model/Stmt/If.hpp"
#include "Model/Stmt/While.hpp"
#include "Model/Stmt/Repeat.hpp"
#include "Model/Stmt/For.hpp"
#include "Model/Stmt/With.hpp"

namespace Parsers {

    // stmt-list := stmt
    // stmt-list := stmt-list T_SEMICOLON stmt
    bool StmtList::operator()() {
        PARSER_FLOW_TRACER();

        Model::StmtList::Ptr m = model();

        // stmt list actually means, parse stmt + T_SEMICOLONopt and repeat until T_SEMICOLON is missing
        for(;;) {
            Stmt sp(this);
            FAIL_IF_NOT(sp());

            Model::Stmt::Ptr stmt = sp.model();
            if (stmt) {
                m->append(stmt);
            }

            if (!isAhead(T_SEMICOLON)) {
                return true;
            }
            REQUIRE_SEMICOLON();
        }
    }

}
