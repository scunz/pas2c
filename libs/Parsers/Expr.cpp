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

#include "Model/Expr/Binary.hpp"
#include "Model/Expr/Unary.hpp"

#include "Model/Expr/Const/Const.hpp"
#include "Model/Expr/Const/String.hpp"
#include "Model/Expr/Const/Number.hpp"
#include "Model/Expr/Const/Float.hpp"
#include "Model/Expr/LValue.hpp"

namespace Parsers {

    static inline bool IS_ADDI_EXPR_OPER(TokenType t) {
        switch(t) {
        case T_PLUS:
        case T_MINUS:
        case T_OR:
            return true;
        default:
            return false;
        }
    }

    static inline bool IS_MULTI_EXPR_OPER(TokenType t) {
        switch(t) {
        case T_ASTERIKS:
        case T_SLASH:
        case T_AND:
        case T_IN:
        case T_DIV:
        case T_MOD:
            return true;
        default:
            return false;
        }
    }

    static inline Model::UnaryOperator tokenToUnary(TokenType t) {
        switch(t) {
        case T_AT_SIGN:             return Model::operAddrOf;
        case T_NOT:                 return Model::operNot;
        case T_PLUS:                return Model::operKeepSign;
        case T_MINUS:               return Model::operToggleSign;
        default:                    return Model::operNoneUnary;
        }
    }

    static inline Model::BinaryOperator tokenToBinary(TokenType t) {
        switch(t) {
        case T_LESS_THAN_EQUAL:     return Model::operLessEqual;
        case T_GREATER_THAN_EQUAL:  return Model::operGreaterEqual;
        case T_LEFT_ANGLE:          return Model::operLess;
        case T_RIGHT_ANGLE:         return Model::operGreater;
        case T_EQUAL:               return Model::operEqual;
        case T_INEQUAL:             return Model::operInequal;
        case T_ASTERIKS:            return Model::operMult;
        case T_SLASH:               return Model::operDivision;
        case T_AND:                 return Model::operAnd;
        case T_IN:                  return Model::operIn;
        case T_DIV:                 return Model::operIntDiv;
        case T_MOD:                 return Model::operIntMod;
        case T_PLUS:                return Model::operAdd;
        case T_MINUS:               return Model::operMinus;
        case T_OR:                  return Model::operOr;
        default:                    return Model::operNoneBinary;
        }
    }

    bool Expr::operator()() {
        PARSER_FLOW_TRACER();

        FAIL_IF_NOT(parseExpression());
        return true;
    }

    bool Expr::parseExpression() {
        PARSER_FLOW_TRACER();
        FAIL_IF_NOT(parseRelationalExpression());
        return true;
    }

    bool Expr::parseRelationalExpression() {
        PARSER_FLOW_TRACER();
        /* right associative */

        FAIL_IF_NOT(parseAdditiveExpression());

        switch(ahead().type()) {
        case T_LESS_THAN_EQUAL:
        case T_GREATER_THAN_EQUAL:
        case T_LEFT_ANGLE:
        case T_RIGHT_ANGLE:
        case T_EQUAL:
        case T_INEQUAL: {
            Model::ExprBinary::Ptr result = new Model::ExprBinary;
            result->setLeftOperand(model());
            result->setBinaryOperator(tokenToBinary(ahead().type()));

            next();
            FAIL_IF_NOT(parseAdditiveExpression());
            result->setRightOperand(model());

            setModel(result); }
        }

        return true;
    }

    bool Expr::parseAdditiveExpression() {
        PARSER_FLOW_TRACER();
        /* right associative */

        FAIL_IF_NOT(parseMultiplicativeExpression());

        while (IS_ADDI_EXPR_OPER(ahead().type())) {
            Model::ExprBinary::Ptr result = new Model::ExprBinary;
            result->setLeftOperand(model());
            result->setBinaryOperator(tokenToBinary(ahead().type()));

            next();
            FAIL_IF_NOT(parseMultiplicativeExpression());
            result->setRightOperand(model());

            setModel(result);
        }

        return true;
    }

    bool Expr::parseMultiplicativeExpression() {
        PARSER_FLOW_TRACER();
        /* right associative */

        FAIL_IF_NOT(parseUnaryExpression());

        while (IS_MULTI_EXPR_OPER(ahead().type())) {
            Model::ExprBinary::Ptr result = new Model::ExprBinary;
            result->setLeftOperand(model());
            result->setBinaryOperator(tokenToBinary(ahead().type()));

            next();
            FAIL_IF_NOT(parseUnaryExpression());
            result->setRightOperand(model());

            setModel(result);
        }

        return true;
    }

    bool Expr::parseUnaryExpression() {
        PARSER_FLOW_TRACER();
        /* left associative */

        switch (ahead().type()) {
        case T_AT_SIGN:
            emitError("addr-of-operator unsupported");
            // ### @foo (addr-of operator)
            return false;

        case T_MINUS:
        case T_PLUS:
        case T_NOT: {
            Model::ExprUnary::Ptr result = new Model::ExprUnary;
            result->setUnaryOperator(tokenToUnary(ahead().type()));
            next();
            FAIL_IF_NOT(parsePrimaryExpression());
            result->setOperand(model());
            setModel(result);
            return true; }

        default:
            return parsePrimaryExpression();
        }
    }

    bool Expr::parsePrimaryExpression() {
        PARSER_FLOW_TRACER();

        switch(ahead().type()) {
        case T_LEFT_PAREN: {
            REQUIRE_TOKEN(T_LEFT_PAREN);
            FAIL_IF_NOT(parseExpression());
            REQUIRE_TOKEN(T_RIGHT_PAREN);
            return true; }

        case T__NUMBER: {
            next();
            setModel(new Model::ExprConstNumber(current().number()));
            return true; }

        case T__STRING: {
            next();
            setModel(new Model::ExprConstString(QString::fromUtf8(current().text().constData())));
            return true; }

        case T__FLOAT:
            next();
            setModel(new Model::ExprConstDouble(current().numberFloat()));
            return true;

        case T__IDENTIFIER: {

            Model::Decl::Ptr decl = curCtx()->lookup(ahead().identifier());
            if (!decl) {
                emitError(QLatin1Literal("Unknown identifier: ") %
                          Model::Identifier(ahead().identifier()).toString());
                return false;
            }

            if (decl->type() == Model::oDeclConst) {
                next();

                Model::ExprConstConst::Ptr ecc = new Model::ExprConstConst;
                ecc->setDecl(decl.scast<Model::DeclConst>());
                setModel(ecc);

                return true;
            }

            LValue lvp(this);
            lvp.setOptions(LValue::AllowFunctionCall);
            FAIL_IF_NOT(lvp());

            Model::ExprLValue::Ptr elv = new Model::ExprLValue;
            elv->setLValue(lvp.model());
            setModel(elv);
            return true; }

        default:
            return false;
        }
    }

}
