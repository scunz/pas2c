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

#include <QStringBuilder>

#include "Pascal.hpp"
#include "Unit.hpp"
#include "Program.hpp"

namespace Parsers {

    Pascal* Pascal::create(const ContextStack::Ptr& ctxStack,
                           Errors::Ptr errs,
                           const TokenStream::Ptr& stream)
    {
        Pascal* result = NULL;

        switch (stream->ahead().type()) {
        case T_PROGRAM:
            result = new Program(ctxStack, errs, stream);
            break;

        case T_UNIT:
            result = new Unit(ctxStack, errs, stream);
            break;

        default:
            stream->ahead().inputStreamRef().emitError(errs, "Expected 'program' or 'unit'");
            break;
        }

        return result;
    }

    void Pascal::setProject(Project::Ptr project) {
        mProject = project;
    }

    Project::Ptr Pascal::project() const {
        return mProject;
    }

    bool Pascal::parseUses(bool forInterface) {
        PARSER_FLOW_TRACER();

        REQUIRE_TOKEN(T_USES);

        for(;;) {
            REQUIRE_TOKEN(T__IDENTIFIER);

            Model::RootUnit::Ptr unit = project()->getOrParseUnit(current().identifier());
            if (!unit) {
                emitError(QLatin1Literal("Cannot use unit '") %
                          curIdentifier().toString() %
                          QLatin1Literal("'. Source was not found."));
                return false;
            }
            importUnit(unit, forInterface);

            if (!isAhead(T_COMMA)) {
                REQUIRE_SEMICOLON();
                return true;
            }

            REQUIRE_TOKEN(T_COMMA);
        }
    }

}
