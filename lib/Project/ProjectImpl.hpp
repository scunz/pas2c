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

#ifndef PASC_PROJECT_HPP
#define PASC_PROJECT_HPP
#pragma once

#include <QStringList>

// CoreUtils
#include "PtrBase.hpp"
#include "InputStream.hpp"
#include "InputStreamCollection.hpp"

// Interfaces
#include "Project.hpp"

// LexAnal
#include "LexicalAnalysis.hpp"

// CodeModel
#include "Model/Root/Unit.hpp"

// Project
#include "ProjectAPI.hpp"

class QFile;

class PASC_PROJECT_API ProjectImpl : public Project
{
public:
    typedef PtrT<ProjectImpl> Ptr;

public:
    enum ProjectOption {
        optNone                         = 0,

        optDumpTokenStreams             = 1UL << 0,
        optDumpParserFlow               = 1UL << 1,
        optDumpCodeModel                = 1UL << 2,
        optDumpInputStats               = 1UL << 3,
        optDumpGlobalStats              = 1UL << 4,

    };
    typedef QFlags<ProjectOption> ProjectOptions;

    enum PascalModes {
        pmAncient,
        pmTurbo,
        pmDelphi
    };

public:
    ProjectImpl();
    ~ProjectImpl();

public:
    bool parseArguments(QStringList args);
    bool initialize();
    bool run();

public:
    void setOption(ProjectOption opt, bool set = true);
    bool testOption(ProjectOption opt) const;
    PascalModes mode() const;

public:
    Errors::Ptr errors();
    bool hasErrors();
    void dumpErrors();

public:
    void addUnit(Model::RootUnit::Ptr unit);
    Model::RootUnit::Ptr getOrParseUnit(const Model::Identifier& name);
    Model::RootUnit::Ptr getUnit(const Model::Identifier& name);

    void emitError(const char* text, const InputStreamRef& ref);
    void emitError(const QString& text, const InputStreamRef& ref);

    InputStream::Ptr inputStreamFor(const QString& fileName);

private:
    void setFactory(Factories type, FactoryBase* factory);
    FactoryBase* factory(Factories type) const;
    void printUsage();
    QString locateFile(const QStringList& paths, const QStringList& exts,
                       const QString& file, bool tryBasenames = false) const;
    QString locateSourceFile(const QString& fileNameFragment) const;
    LexicalAnalyzer::Ptr analyze(const QString& fileName);
    Model::Root::Ptr parseFile(const QString& fileName);
    Model::Root::Ptr parseStream(TokenStream::Ptr stream);

private:
    Errors::Ptr                 mErrors;
    ProjectOptions              mOpts;
    PascalModes                 mMode;
    QFile*                      mFlowFile;
    QString                     mInputPath;
    QString                     mInputFile;
    QString                     mFlowFileName;
    QStringList                 mIncludePaths;
    QStringList                 mUnitPaths;
    QString                     mOutputPath;
    InputStreamCollection       mInputStreams;
    QHash<Factories,
          FactoryBase*>         mFactories;
    QHash<Model::Identifier,
          Model::RootUnit::Ptr> mUnits;
};

#endif
