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

#include "Unit.hpp"
#include "DeclList.hpp"

namespace Parsers {

    // pascal := name decls T_INTERFACE interface T_IMPLEMENTATION impl body T_END T_DOT
    bool Unit::operator ()() {
        PARSER_FLOW_TRACER();

        Model::RootUnit::Ptr m = model();

        REQUIRE_TOKEN(T_UNIT);
        FAIL_IF_NOT(parseName());

        REQUIRE_TOKEN(T_INTERFACE);

        if (isAhead(T_USES)) {
            FAIL_IF_NOT(parseUses(true));
        }

        DeclList dpInterface(this);
        dpInterface.setOptions(DeclList::ForInterface);
        FAIL_IF_NOT(dpInterface());
        m->setDeclList(dpInterface.model());
        curCtx()->append(dpInterface.model());

        REQUIRE_TOKEN(T_IMPLEMENTATION);

        if (isAhead(T_USES)) {
            FAIL_IF_NOT(parseUses(false));
        }

        DeclList dpImplementation(this);
        dpImplementation.setOptions(DeclList::ForImplementation);
        FAIL_IF_NOT(dpImplementation());
        m->setImplList(dpImplementation.model());
        curCtx()->append(dpImplementation.model());

        SUCCESS();
    }

    // name := T_UNIT T__IDENTIFIER T_SEMICOLON
    bool Unit::parseName() {
        PARSER_FLOW_TRACER();

        Model::RootUnit::Ptr m = model();

        EXPECT_IDENTIFIER();
        m->setName(current().identifier());
        REQUIRE_SEMICOLON();
        SUCCESS();
    }

    void Unit::importUnit(Model::RootUnit::Ptr unit, bool forInterface) {
        Q_ASSERT(false);
    }

}
