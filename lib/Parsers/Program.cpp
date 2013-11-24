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

#include "Program.hpp"

namespace Parsers {

    // pascal := name decls T_BEGIN body T_END T_DOT
    bool Program::operator ()() {
        PARSER_FLOW_TRACER();

        Model::RootProgram::Ptr m = model();

        FAIL_IF_NOT(parseName());

        SUCCESS();
    }

    // name := T_PROGRAM T__IDENTIFIER T_SEMICOLON
    bool Program::parseName() {
        PARSER_FLOW_TRACER();

        Model::RootProgram::Ptr m = model();

        REQUIRE_TOKEN(T_PROGRAM);
        EXPECT_IDENTIFIER();
        m->setName(current().identifier());
        REQUIRE_SEMICOLON();
        SUCCESS();
    }

    void Program::importUnit(Model::RootUnit::Ptr unit, bool forInterface) {
        Q_ASSERT(false);
    }

}
