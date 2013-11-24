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

#include "Type.hpp"
#include "IdentifierList.hpp"

#include "Model/Decl/Type.hpp"
#include "Model/Type/Stock.hpp"
#include "Model/Type/Enum.hpp"
#include "Model/Type/File.hpp"
#include "Model/Type/Pointer.hpp"
#include "Model/Type/Record.hpp"
#include "Model/Type/Custom.hpp"
#include "Model/Type/CustomAhead.hpp"

namespace Parsers {

    bool Type::operator ()() {
        PARSER_FLOW_TRACER();

        switch(ahead().type()) {
        case T__IDENTIFIER: {
            REQUIRE_TOKEN(T__IDENTIFIER);
            Model::Identifier name = current().identifier();

            QSet<Model::Objects> declTypes;
            declTypes << Model::oDeclType;

            Model::Decl::Ptr decl = curCtx()->lookupRestricted(name, declTypes);
            if (!decl) {
                if (testOption(tpoAllowAheadReference)) {

                    /* ### We cannot test if the name is only in current DeclList...
                    if (curCtx()->lookupNonRecursive(name)) {
                        emitError(QLatin1Literal("Cannot introduce identifier as forward decl "
                                                 "type name: ") % name.toString());
                        return false;
                    }
                    */

                    // we can neither create a Model::TypeCustom (to store the reference) nor a new
                    // Model::DeclType (to store the forward-type) here, since we have to have the
                    // DeclType to create the TypeCustom (which we would have to return) and we cannot
                    // create the DeclType, since we don't have access to the owning Model::DeclList.

                    // However, we need to create something, since we might be wrapped in whatever type-
                    // decls. We have to later deeply climb down that "whatever" and create the real
                    // TypeCustom.
                    Model::TypeCustomAhead* ahead = new Model::TypeCustomAhead;
                    mAheadName = name;
                    ahead->setDecl(name);
                    setModel(ahead);

                    return true;
                }
                else {
                    emitError(QLatin1Literal("Expected type identifier. Instead got '") %
                              name.toString() %
                              QLatin1Literal("' and only god knows what that is..."));
                    return false;
                }
            }

            // Convert the found identifer to a DeclType and create a TypeCustom-Reference to it.
            Model::DeclType::Ptr declType = decl.scast<Model::DeclType>();
            Model::TypeCustom::Ptr tc = new Model::TypeCustom;
            tc->setDecl(declType);
            setModel(tc);

            return true; }

        case T_BOOLEAN:
            REQUIRE_TOKEN(T_BOOLEAN);
            setModel(new Model::TypeStock(Model::dtBoolean));
            return true;

        case T_FLOAT:
            REQUIRE_TOKEN(T_FLOAT);
            setModel(new Model::TypeStock(Model::dtFloat));
            return true;

        case T_INTEGER:
            REQUIRE_TOKEN(T_INTEGER);
            setModel(new Model::TypeStock(Model::dtInteger));
            return true;

        case T_CHAR:
            REQUIRE_TOKEN(T_CHAR);
            setModel(new Model::TypeStock(Model::dtChar));
            return true;

        case T_POINTER:
            REQUIRE_TOKEN(T_POINTER);
            setModel(new Model::TypeStock(Model::dtPointer));
            return true;

        case T_TEXT:
            REQUIRE_TOKEN(T_TEXT);
            setModel(new Model::TypeStock(Model::dtText));
            return true;

        case T_CARET: {
            REQUIRE_TOKEN(T_CARET);

            Type tp(this);
            tp.setOptions(options());
            FAIL_IF_NOT(tp());
            setModel(new Model::TypePointerOf(tp.model()));

            mAheadName = tp.aheadName();

            return true; }

        case T_STRING:
            REQUIRE_TOKEN(T_STRING);
            if (isAhead(T_LEFT_SQUARE)) {
                // ### "string[n]" --> Needs special model!
                REQUIRE_TOKEN(T_LEFT_SQUARE);
                // ### ConstExprParser
                REQUIRE_TOKEN(T__NUMBER);
                REQUIRE_TOKEN(T_RIGHT_SQUARE);
                qDebug() << "Ignoreing String length predicate";
            }
            setModel(new Model::TypeStock(Model::dtString));
            return true;

        case T_FILE: {
            REQUIRE_TOKEN(T_FILE);
            if (isAhead(T_OF)) {
                REQUIRE_TOKEN(T_OF);
                Type tp(this);
                tp.setOptions(options());
                FAIL_IF_NOT(tp());
                setModel(new Model::TypeFileOf(tp.model()));

                mAheadName = tp.aheadName();
            }
            else {
                setModel(new Model::TypeStock(Model::dtFile));
            }
            return true; }

        //case T_PACKED:
        //case T_BITPACKED:
        case T_RECORD:
            FAIL_IF_NOT(parseRecord());
            return true;

        case T_LEFT_PAREN:
            // A left-parent could indicate a enumeration declaration but could as well be part of a
            // const-expr that starts a range-decl. If the sequence is
            //          T_LEFT_PAREN T__IDENTIFIER T_COMMA|T_RIGHT_PARENT
            // then we assume an enum-decl.
            if (isAhead(T__IDENTIFIER,2) && (isAhead(T_COMMA,3)||isAhead(T_RIGHT_PAREN,3))) {
                REQUIRE_TOKEN(T_LEFT_PAREN);
                // it's an enum-decl
                Model::TypeEnum::Ptr te = new Model::TypeEnum;
                for(;;) {
                    EXPECT_IDENTIFIER();
                    te->addEnumerator(current().identifier());
                    if (!isAhead(T_COMMA)) {
                        break;
                    }
                    EXPECT_TOKEN(T_COMMA);
                }
                REQUIRE_TOKEN(T_RIGHT_PAREN);

                setModel(te);
                return true;
            }
            // else fall through
        default: {
            // finally check for:
            //      const-expr T_DOUBLE_DOT const-expr
            // ### RangeExprParser
            }
        }

        return false;
    }

    bool Type::parseRecordFieldList() {
        PARSER_FLOW_TRACER();

        Model::TypeRecord::Ptr m = model().scast<Model::TypeRecord>();

        for(;;) {
            IdentifierList ilp(this);
            FAIL_IF_NOT(ilp());
            EXPECT_TOKEN(T_COLON);
            Type tp(this);
            FAIL_IF_NOT(tp());

            foreach (const Model::Identifier& name, ilp.list()) {
                m->addField(name, tp.model());
            }

            if (!isAhead(T__IDENTIFIER)) {
                return true;
            }
        }
    }

    // FROM: http://www.freepascal.org/docs-html/ref/refsu19.html

    // record-decl          := pack-spec T_RECORD field-decl-list T_END
    // pack-spec            := /**/
    // field-decl-list      := field-decl-list-1
    // field-decl-list      := field-decl-list-1 T_SEMICOLON
    // field-decl-list-1    := fixed-field-list
    // fixed-field-list     := id-list T_COLON type-ref
    // fixed-field-list     := id-list T_COLON type-ref T_SEMICOLON fixed-field-list
    //--- unsupported:
    // pack-spec            := T_PACKED
    // pack-spec            := T_BITPACKED
    // field-decl-list-1    := fixed-field-list T_SEMICOLON variant-part
    // variant-part         := T_CASE varient-identifier ord-type-ref T_OF variants-list
    // variants-list        := variant
    // variants-list        := variant T_SEMICOLON variants-list
    // variant              := constant-list T_COLON T_LEFT_PARENT field-list T_RIGHT_PAREN
    // constant-list        := const-expr
    // constant-list        := const-expr T_COMMA constant-list
    // variant-identifier   := /**/
    // variant-identifier   := T__IDENTIFIER T_COLON
    bool Type::parseRecord() {
        PARSER_FLOW_TRACER();

        Model::TypeRecord::Ptr m = new Model::TypeRecord;
        setModel(m);

        /*
        if (isAhead(T_PACKED)) {
            REQUIRE_TOKEN(T_PACKED);
            m->setMode(recPacked);
        }
        else if (isAhead(T_BITPACKED)) {
            REQUIRE_TOKEN(T_BITPACKED);
            m->setMode(recBitPacked);
        }
        */

        REQUIRE_TOKEN(T_RECORD);
        do {
            FAIL_IF_NOT(parseRecordFieldList());
            if (isAhead(T_SEMICOLON)) {
                REQUIRE_SEMICOLON();
            }
        } while (!isAhead(T_END));
        REQUIRE_TOKEN(T_END);
        return true;
    }

}
