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

#include "Stmt.hpp"
#include "Expr.hpp"
#include "StmtList.hpp"
#include "LValue.hpp"

#include "Model/Stmt/Compound.hpp"
#include "Model/Stmt/If.hpp"
#include "Model/Stmt/While.hpp"
#include "Model/Stmt/Repeat.hpp"
#include "Model/Stmt/For.hpp"
#include "Model/Stmt/With.hpp"
#include "Model/Stmt/Assign.hpp"

namespace Parsers {

    bool Stmt::operator()() {
        PARSER_FLOW_TRACER();

        switch(ahead().type()) {
        case T_BEGIN:
            FAIL_IF_NOT(parseCompound());
            return true;

        case T_IF:
            FAIL_IF_NOT(parseIf());
            return true;

        case T_WHILE:
            FAIL_IF_NOT(parseWhile());
            return true;

        case T_FOR:
            FAIL_IF_NOT(parseFor());
            return true;

        case T_REPEAT:
            FAIL_IF_NOT(parseRepeat());
            return true;

        case T_CASE:
            FAIL_IF_NOT(parseCase());
            return true;

        case T_WITH:
            FAIL_IF_NOT(parseWith());
            return true;

        case T_LEFT_PAREN:
        case T__IDENTIFIER: {
            LValue lvp(this);
            FAIL_IF_NOT(lvp());

            if (isAhead(T_ASSIGNMENT)) {
                FAIL_IF_NOT(parseAssignment(lvp.model()));
            }
            else /* if (isAhead(T_LEFT_PAREN)) */{
                FAIL_IF_NOT(parseProcCall(lvp.model()));
            } /* Pascal doesn't reqiure the PAREN... We cannot diagnose this error...
            else {
                emitError("Expected assignment or proc-call");
                return false;
            }*/
            return true;
        }

        default:
            return true;    // stmt can be empty!
        }
    }

    bool Stmt::parseIf() {
        PARSER_FLOW_TRACER();

        Model::StmtIf::Ptr stmt = new Model::StmtIf;

        REQUIRE_TOKEN(T_IF);
        Expr ep(this);
        FAIL_IF_NOT(ep());
        stmt->setExpr(ep.model());

        REQUIRE_TOKEN(T_THEN);

        Stmt sp(this);
        FAIL_IF_NOT(sp());
        stmt->setTrueStatement(sp.model());

        if (isAhead(T_ELSE)) {
            REQUIRE_TOKEN(T_ELSE);

            Stmt sp2(this);
            FAIL_IF_NOT(sp2());
            stmt->setFalseStatement(sp2.model());
        }

        setModel(stmt);
        return true;
    }

    // stmt-compound := T_BEGIN stmt-list T_END
    bool Stmt::parseCompound() {
        PARSER_FLOW_TRACER();

        REQUIRE_TOKEN(T_BEGIN);
        StmtList slp(this);
        FAIL_IF_NOT(slp());
        REQUIRE_TOKEN(T_END);

        Model::StmtCompound::Ptr stmt = new Model::StmtCompound;
        stmt->setStatements(slp.model());
        setModel(stmt);

        return true;
    }

    // stmt-for := T_FOR stmt-assign T_TO     expr T_DO stmt
    // stmt-for := T_FOR stmt-assign T_DOWNTO expr T_DO stmt
    bool Stmt::parseFor() {
        PARSER_FLOW_TRACER();

        REQUIRE_TOKEN(T_FOR);
        LValue lvp(this);
        FAIL_IF_NOT(lvp());

        EXPECT_TOKEN(T_ASSIGNMENT);
        Model::Stmt::Ptr assiStmt;
        FAIL_IF_NOT(parseAssignment2(lvp.model(), assiStmt));

        bool upWards = false;
        if (isAhead(T_TO)) {
            upWards = true;
            REQUIRE_TOKEN(T_TO);
        }
        else {
            REQUIRE_TOKEN(T_DOWNTO);
        }

        Expr ep(this);
        FAIL_IF_NOT(ep());

        REQUIRE_TOKEN(T_DO);

        Stmt sp(this);
        FAIL_IF_NOT(sp());

        // ### FOR code model

        return false;
    }

    bool Stmt::parseWhile() {
        PARSER_FLOW_TRACER();

        REQUIRE_TOKEN(T_WHILE);

        Expr ep(this);
        FAIL_IF_NOT(ep());

        REQUIRE_TOKEN(T_DO);

        Stmt sp(this);
        FAIL_IF_NOT(sp());

        // ### While Code Model

        return true;
    }

    bool Stmt::parseRepeat() {
        PARSER_FLOW_TRACER();

        REQUIRE_TOKEN(T_REPEAT);

        StmtList slp(this);
        FAIL_IF_NOT(slp());

        REQUIRE_TOKEN(T_UNTIL);

        Expr ep(this);
        FAIL_IF_NOT(ep());

        Model::StmtRepeat::Ptr stmt = new Model::StmtRepeat;
        stmt->setExpr(ep.model());
        stmt->setStmtList(slp.model());
        setModel(stmt);

        return true;
    }

    bool Stmt::parseWith() {
        PARSER_FLOW_TRACER();

        // ### StmtParser::parseWith

        return false;
    }

    bool Stmt::parseCase() {
        PARSER_FLOW_TRACER();

        // ### StmtParser::parseCase

        return false;
    }

    bool Stmt::parseProcCall(Model::LValue::Ptr lvalue) {
        PARSER_FLOW_TRACER();

        // ### StmtParser::parseProcCall
        return false;
    }

    bool Stmt::parseAssignment2(Model::LValue::Ptr lvalue, Model::Stmt::Ptr& stmt) {
        PARSER_FLOW_TRACER();

        REQUIRE_TOKEN(T_ASSIGNMENT);
        Expr ep(this);
        FAIL_IF_NOT(ep());

        Model::StmtAssign::Ptr astmt = new Model::StmtAssign;
        astmt->setLValue(lvalue);
        astmt->setExpr(ep.model());

        stmt = astmt;
        return true;
    }

    bool Stmt::parseAssignment(Model::LValue::Ptr lvalue) {
        PARSER_FLOW_TRACER();

        Model::Stmt::Ptr assiStmt;
        FAIL_IF_NOT(parseAssignment2(lvalue, assiStmt));

        setModel(assiStmt);
        return true;
    }

}
