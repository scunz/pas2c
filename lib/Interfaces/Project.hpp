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

#ifndef PASC_IF_PROJECT_HPP
#define PASC_IF_PROJECT_HPP
#pragma once

// CoreUtils
#include "PtrBase.hpp"
#include "InputStream.hpp"
#include "InputStreamRef.hpp"

// Interfaces
#include "InterfacesAPI.hpp"
#include "LexicalAnalysis.hpp"
#include "TokenTypes.hpp"

class FactoryBase {
public:
    virtual PtrBase* create() = 0;
};

enum Factories {
    factLexicalAnalyer,
    factParser
};

namespace Model {
    class Identifier;
    class TypeStock;
    class RootUnit;
}

// This is somewhat an "application" class
class PASC_INTERFACES_API Project : public PtrBase {
public:
    typedef PtrT<Project> Ptr;

public:
    virtual Errors::Ptr errors() const = 0;
    virtual InputStream::Ptr inputStreamFor(const QString& fileName) = 0;
    virtual PtrT<Model::RootUnit> getOrParseUnit(const Model::Identifier& name) = 0;

    virtual PtrT<Model::TypeStock> stockType(TokenType tokenType) = 0;

protected:
    virtual void setFactory(Factories type, FactoryBase* factory) = 0;
    virtual FactoryBase* factory(Factories type) const = 0;

public:
    void emitError(const char* text, const InputStreamRef& ref);
    void emitError(const QString& text, const InputStreamRef& ref);
    LexicalAnalyzer::Ptr newAnalyzer(const QString& fileName);
};

#endif
