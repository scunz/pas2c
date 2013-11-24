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

#ifndef PASC_BASE_PARSER_HPP
#define PASC_BASE_PARSER_HPP

// CoreUtils
#include "Errors.hpp"
#include "FlowTracer.hpp"
#include "InputStreamRef.hpp"
#include "Token.hpp"
#include "TokenStream.hpp"

// Interfaces
#include "Project.hpp"

// CodeModel
#include "Model/Code.hpp"

// Parsers
#include "Context.hpp"

namespace Parsers {

    class Base
    {
    protected:
        Base(const ContextStack::Ptr&   ctxStack,
             const Project::Ptr&        project,
             const TokenStream::Ptr&    stream);

        Base(Base* father)
            : mCtxStack(father->mCtxStack)
            , mProject(father->mProject)
            , mStream(father->mStream)
        {}

    protected:
        const Token& current() const                        { return mStream->current(); }
        const Token& next() const                           { return mStream->next(); }
        const Token& ahead(int count = 1) const             { return mStream->ahead(count); }
        bool isAhead(TokenType type, int count = 1) const   { return mStream->isAhead(type,count); }
        bool require(TokenType type) const                  { return mStream->require(type); }

        Model::Identifier curIdentifier() const { return Model::Identifier(current().identifier()); }

    protected:
        const InputStreamRef& pos() const { return ahead().inputStreamRef(); }

    public:
        Model::Code::Ptr model() { return mModel; }
        ContextStack::Ptr contextStack() { return mCtxStack; }
        Context::Ptr curCtx() { return mCtxStack->top(); }
        TokenStream::Ptr stream() { mStream; }

    protected:
        void setModel(Model::Code::Ptr model) { mModel = model; }

    protected:
        void emitError(const QString& text);
        void emitError(const QString& text, const InputStreamRef& ref);
        void emitError(const char* text);
        void emitError(const char* text, const InputStreamRef& ref);

    public:
        Project::Ptr project() const;
        Errors::Ptr errors() const;

    private:
        ContextStack::Ptr   mCtxStack;
        Project::Ptr        mProject;
        TokenStream::Ptr    mStream;
        Model::Code::Ptr    mModel;
    };

}

#define DECL_PARSER_INIT_WITH_MODEL(CLASS,BASE,MODEL) \
    public: \
        CLASS(Base* father) \
            : BASE(father) \
        { \
            setModel(new Model::MODEL); \
        } \
        Model::MODEL::Ptr model() \
            { return static_cast<Model::MODEL*>(BASE::model().data()); }

#define DECL_PARSER_INIT_WITHOUT_MODEL(CLASS,BASE,MODEL) \
    public: \
        CLASS(Base* father) \
            : BASE(father) \
            { } \
        Model::MODEL::Ptr model() \
            { return static_cast<Model::MODEL*>(BASE::model().data()); }

#define REQUIRE_TOKEN(t) do { \
    if (!require((t))) { \
        emitError("Expected " #t " token."); \
        return false; } \
    } while(0)

#define EXPECT_TOKEN(t) do { \
    if (next().type() != (t)) { \
        emitError("Expected " #t " token."); \
        return false; } \
    } while(0)

#define EXPECT_IDENTIFIER()     EXPECT_TOKEN(T__IDENTIFIER)
#define REQUIRE_SEMICOLON()     REQUIRE_TOKEN(T_SEMICOLON)
#define REQUIRE_COLON()         REQUIRE_TOKEN(T_COLON)

#define FAIL_IF_NOT(x) do { if (!(x)) { return false; } } while(0)
#define SUCCESS() return true;

#define PARSER_FLOW_TRACER() \
    FLOW_TRACER_EX(pos().description())

#endif
