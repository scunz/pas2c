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

#include "LValue.hpp"
#include "Expr.hpp"

#include "Model/Decl/Type.hpp"

#include "Model/Type/Stock.hpp"

#include "Model/LValue/VarRef.hpp"
#include "Model/LValue/SubscriptDeref.hpp"
#include "Model/LValue/UnitDeref.hpp"
#include "Model/LValue/RecordDeref.hpp"
#include "Model/LValue/PointerDeref.hpp"
#include "Model/LValue/TypeCast.hpp"

namespace Parsers {

    bool LValue::operator ()() {
        PARSER_FLOW_TRACER();

        switch (ahead().type()) {
        case  T_LEFT_PAREN: {

            // We keep the L-PAREN on the stream, so ExprParser will find a ()-Expr.
            Expr ep(this);
            FAIL_IF_NOT(ep());

            return false; }

        case T_STRING:
            // ### String needs special handling here, probably
        case T_INTEGER:
        case T_FLOAT:
        case T_BOOLEAN:
        case T_FILE:
        case T_POINTER:
        case T_TEXT: {
            Model::TypeStock* type = project()->stockType(ahead().type());
            next();
            FAIL_IF_NOT(parseTypeCast(type));
            return true;
        }

        case T__IDENTIFIER: {
            EXPECT_IDENTIFIER();
            Model::Identifier id = current().identifier();

            Model::Decl::Ptr decl = curCtx()->lookup(id);
            if (!decl) {
                emitError(QLatin1Literal("Unknown Identifier: ") % id.toString());
                return false;
            }

            switch(decl->type()) {
            case Model::oDeclType:
                REQUIRE_TOKEN(T__IDENTIFIER); // eat the identifier
                FAIL_IF_NOT(parseTypeCast(decl.scast<Model::DeclType>()->dataType()));
                return true;

            case Model::oDeclVar:
                FAIL_IF_NOT(parseVarRef(decl.scast<Model::DeclVar>()));
                return true;

            case Model::oDeclProc:
                if (testOption(AllowProcedureCall)) {
                    // ### Proc-Call
                }
                else {
                    emitError("Procedure call not allowed here.");
                    return false;
                }
                return false;

            case Model::oDeclFunc:
                if (testOption(AllowProcedureCall)) {
                    // ### Func-Call
                }
                else {
                    emitError("Function call not allowed here.");
                    return false;
                }
                return false;

            case Model::oDeclUses:
                break;

            default:
                emitError(QLatin1Literal("Don't know what to do with this identifier here: ") %
                          id.toString());
            }
            return false; }
        }

        return false;
    }

    bool LValue::parseVarRef(const Model::DeclVar::Ptr& decl) {
        PARSER_FLOW_TRACER();

        Model::LValueVarRef::Ptr vr = new Model::LValueVarRef;
        vr->setVar(decl);

        return parseRightAssoced(vr);
    }

    bool LValue::parseTypeCast(const Model::Type::Ptr& type) {
        PARSER_FLOW_TRACER();

        REQUIRE_TOKEN(T_LEFT_PAREN);

        LValue lvp(this);
        FAIL_IF_NOT(lvp());

        REQUIRE_TOKEN(T_RIGHT_PAREN);

        Model::LValueTypeCast::Ptr tc = new Model::LValueTypeCast;
        tc->setSource(lvp.model());
        tc->setDataType(type);

        return parseRightAssoced(tc);
    }

    bool LValue::parseRightAssoced(const Model::LValue::Ptr& lv) {
        PARSER_FLOW_TRACER();

        switch(ahead().type()) {
        case T_CARET:
            return parsePointerDeref(lv);

        case T_DOT:
            return parseRecordDeref(lv);

        case T_LEFT_SQUARE:
            return parseArraySubscript(lv);

        default:
            setModel(lv);
            return true;
        }
    }

    bool LValue::parsePointerDeref(const Model::LValue::Ptr& lv) {
        PARSER_FLOW_TRACER();

    }

    bool LValue::parseRecordDeref(const Model::LValue::Ptr& lv) {
        PARSER_FLOW_TRACER();

    }

    bool LValue::parseArraySubscript(const Model::LValue::Ptr& lv) {
        PARSER_FLOW_TRACER();

    }

}
