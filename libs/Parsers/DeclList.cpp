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

#include "DeclList.hpp"
#include "Type.hpp"
#include "IdentifierList.hpp"
#include "StmtList.hpp"
#include "Expr.hpp"

#include "Model/Decl/Proc.hpp"
#include "Model/Decl/Func.hpp"
#include "Model/Decl/Type.hpp"
#include "Model/Decl/Const.hpp"
#include "Model/Decl/Var.hpp"

#include "Model/Type/CustomAhead.hpp"

namespace Parsers {

    // decl-list := /**/
    // decl-list := var-decl decl-list
    // decl-list := const-decl decl-list
    // decl-list := type-decl decl-list
    // decl-list := func-decl decl-list
    // decl-list := proc-decl decl-list
    bool DeclList::operator()() {
        PARSER_FLOW_TRACER();
        Model::DeclList::Ptr m = model();

        Context::Ptr myCtx = Context::inherited(curCtx());
        contextStack()->push(myCtx);
        myCtx->append(m);

        for(;;) {
            switch(ahead().type()) {
            case T_VAR:
                FAIL_IF_NOT(parseVar());
                break;

            case T_CONST:
                FAIL_IF_NOT(parseConst());
                break;

            case T_TYPE:
                FAIL_IF_NOT(parseType());
                break;

            case T_PROCEDURE:
                FAIL_IF_NOT(parseProc());
                break;

            case T_FUNCTION:
                FAIL_IF_NOT(parseFunc());
                break;

            default:
                return true;
            }
        }

    }

    // var-decl         := T_VAR var-single-decl var-decl-list
    // var-decl-list    := /**/
    // var-decl-list    := var-single-decl var-decl-list
    // var-single-decl  := id-list T_COLON type-ref T_SEMICOLON
    bool DeclList::parseVar() {
        PARSER_FLOW_TRACER();

        Model::DeclList::Ptr m = model();

        REQUIRE_TOKEN(T_VAR);

        do {
            IdentifierList ilp(this);
            FAIL_IF_NOT(ilp());
            REQUIRE_COLON();
            Type tp(this);
            FAIL_IF_NOT(tp());
            REQUIRE_SEMICOLON();

            foreach (const Model::Identifier& name, ilp.list()) {

                if (m->has(name)) {
                    Model::Decl::Ptr d = m->get(name);
                    emitError(name.toString() %
                              QLatin1Literal(" is already known as symbol of type: ") %
                              d->typeName());
                    return false;
                }

                Model::DeclVar::Ptr cm = new Model::DeclVar;
                cm->setName(name);
                cm->setDataType(tp.model());

                m->append(cm);
            }

        } while(isAhead(T__IDENTIFIER));   // repeat if there is another T__IDENTIFIER ahead

        return true;
    }

    bool DeclList::parseConst() {
        PARSER_FLOW_TRACER();

        Model::DeclList::Ptr m = model();

        REQUIRE_TOKEN(T_CONST);

        do {
            Model::Type::Ptr type;

            IdentifierList ilp(this);
            FAIL_IF_NOT(ilp());

            if (isAhead(T_COLON)) {
                REQUIRE_COLON();
                Type tp(this);
                FAIL_IF_NOT(tp());
                type = tp.model();
            }

            REQUIRE_TOKEN(T_EQUAL);

            Expr ep(this);
            FAIL_IF_NOT(ep());

            REQUIRE_SEMICOLON();

            Model::Expr::Ptr expr = ep.model();
            if (!expr->isConst()) {
                emitError("Expression in const decl is not constant.");
                return false;
            }

            // ### Const-Decl: Check if expr can be coerced to type (if type is given)

            foreach (const Model::Identifier& name, ilp.list()) {

                if (m->has(name)) {
                    Model::Decl::Ptr d = m->get(name);
                    emitError(name.toString() %
                              QLatin1Literal(" is already known as symbol of type: ") %
                              d->typeName());
                    return false;
                }

                Model::DeclConst::Ptr cm = new Model::DeclConst;
                cm->setName(name);
                cm->setDataType(type);
                cm->setExpr(expr);

                m->append(cm);
            }

        } while(isAhead(T__IDENTIFIER));   // repeat if there is another T__IDENTIFIER ahead

        return true;
    }

    bool DeclList::parseType() {
        PARSER_FLOW_TRACER();

        Model::IdentifierSet aheadDecls;
        Model::DeclList::Ptr m = model();

        REQUIRE_TOKEN(T_TYPE);

        do {
            IdentifierList ilp(this);
            FAIL_IF_NOT(ilp());
            REQUIRE_TOKEN(T_EQUAL);
            Type tp(this);
            tp.setOptions(Type::tpoAllowAheadReference);
            FAIL_IF_NOT(tp());
            REQUIRE_SEMICOLON();

            Model::Type::Ptr type = tp.model();

            if (tp.hasAhead()) {
                const Model::Identifier& name = tp.aheadName();
                if (!aheadDecls.contains(name)) {

                    if (m->has(name)) {
                        // actually, this is not really possible, since type_parser would have
                        // barked loudly already...
                        Model::Decl::Ptr d = m->get(name);
                        if (d->type() == Model::oDeclType) {
                            emitError(QLatin1Literal("Cannot redeclare type: ") %
                                      name.toString());
                            return false;
                        }

                        emitError(name.toString() %
                                  QLatin1Literal(" is already known as symbol of type: ") %
                                  d->typeName());
                        return false;
                    }

                    aheadDecls += name;

                    Model::DeclType::Ptr cm = new Model::DeclType;
                    cm->setName(name);
                    m->append(cm);
                }
                // else we already know that it is ahead...
            }

            foreach (const Model::Identifier& name, ilp.list()) {

                if (aheadDecls.contains(name)) {

                    Model::Decl::Ptr base = m->get(name);
                    if (base->type() != Model::oDeclType) {
                        emitError(QLatin1Literal("Confused with ahead declaration of type") %
                                  name.toString());
                        return false;
                    }

                    Model::DeclType::Ptr t = base.scast<Model::DeclType>();
                    t->setDataType(tp.model());

                    m->replaceAheadTypes(name, t);

                    aheadDecls.remove(name);
                }
                else {
                    if (m->has(name)) {
                        Model::Decl::Ptr d = m->get(name);
                        emitError(name.toString() %
                                  QLatin1Literal(" is already known as symbol of type: ") %
                                  d->typeName());
                        return false;
                    }

                    Model::DeclType::Ptr cm = new Model::DeclType;
                    cm->setName(name);
                    cm->setDataType(tp.model());

                    m->append(cm);
                }
            }

        } while(isAhead(T__IDENTIFIER));   // repeat if there is another T__IDENTIFIER ahead

        foreach (const Model::Identifier& name, aheadDecls) {
            emitError(QLatin1Literal("Ahead declaration not resolved:") % name.toString());
        }

        return aheadDecls.count() == 0; // Failed if there are still ahead declarations.
    }

    bool DeclList::parseBody(Model::DeclRoutine::Ptr routine) {
        PARSER_FLOW_TRACER();

        DeclList dlp(this);
        FAIL_IF_NOT(dlp());
        routine->setDecls(dlp.model());

        REQUIRE_TOKEN(T_BEGIN);
        StmtList slp(this);
        FAIL_IF_NOT(slp());
        routine->setStmtList(slp.model());

        REQUIRE_TOKEN(T_END);
        REQUIRE_SEMICOLON();

        return true;
    }


    bool DeclList::parseProc() {
        PARSER_FLOW_TRACER();

        Model::DeclList::Ptr m = model();
        Model::DeclProc::Ptr cm = new Model::DeclProc;

        REQUIRE_TOKEN(T_PROCEDURE);
        EXPECT_IDENTIFIER();
        cm->setName(current().identifier());

        if (isAhead(T_LEFT_PAREN)) {
            // ...
        }
        REQUIRE_SEMICOLON();

        /* ### Procedure flags
        switch (ahead.type()) {
        case T_INLINE:
        case T_FORWARD:
        }
        */

        m->append(cm);

        if (!testOption(ForInterface) /* ### && !cm->testFlag(Model::ForwardDeclFlag)*/) {
            FAIL_IF_NOT(parseBody(cm));
        }

        return true;
    }

    bool DeclList::parseFunc() {
        PARSER_FLOW_TRACER();

        Model::DeclList::Ptr m = model();
        Model::DeclProc::Ptr cm = new Model::DeclProc;

        REQUIRE_TOKEN(T_PROCEDURE);
        EXPECT_IDENTIFIER();
        cm->setName(current().identifier());

        if (isAhead(T_LEFT_PAREN)) {
            // ...
        }
        REQUIRE_SEMICOLON();

        /* ### Procedure flags
        switch (ahead.type()) {
        case T_INLINE:
        case T_FORWARD:
        }
        */

        m->append(cm);
        return true;
    }

}
