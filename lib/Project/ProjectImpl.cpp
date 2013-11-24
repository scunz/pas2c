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

#include <iostream>

#include <QDir>
#include <QFileInfo>
#include <QStringBuilder>

// CoreUtils
#include "FlowTracer.hpp"

// Parsers
#include "Pascal.hpp"

// LexAnal
#include "LexerFactory.hpp"

#include "ProjectImpl.hpp"

ProjectImpl::ProjectImpl() {
    mFlowFile = NULL;
    mMode = pmAncient;
    setFactory(factLexicalAnalyer, new LexerFactory);
}

ProjectImpl::~ProjectImpl() {
    delete mFlowFile;
    mFlowFile = NULL;
}

void ProjectImpl::setOption(ProjectOption opt, bool set) {
    if (set) {
        mOpts |= opt;
    }
    else {
        mOpts &= ~opt;
    }
}

bool ProjectImpl::testOption(ProjectOption opt) const {
    return mOpts.testFlag(opt);
}

ProjectImpl::PascalModes ProjectImpl::mode() const {
    return mMode;
}

void ProjectImpl::addUnit(Model::RootUnit::Ptr unit) {
    mUnits.insert(unit->name(), unit);
}

Model::RootUnit::Ptr ProjectImpl::getUnit(const Model::Identifier& name) {
    return mUnits.value(name, Model::RootUnit::Ptr());
}

Model::RootUnit::Ptr ProjectImpl::getOrParseUnit(const Model::Identifier& name) {
    if (mUnits.contains(name)) {
        return mUnits.value(name, Model::RootUnit::Ptr());
    }

    return NULL;
}

bool ProjectImpl::parseArguments(QStringList args) {

    while(args.count()) {
        QString arg = args.takeFirst();
        if (arg == QLatin1String("--dump-flow")) {
            setOption(optDumpParserFlow);
        }
        else if (arg == QLatin1String("--dump-models")) {
            setOption(optDumpCodeModel);
        }
        else if (arg == QLatin1String("--dump-streams")) {
            setOption(optDumpTokenStreams);
        }
        else if (arg == QLatin1String("--dump-stats")) {
            setOption(optDumpInputStats);
        }
        else if (arg == QLatin1String("--dump-global")) {
            setOption(optDumpGlobalStats);
        }
        else if (arg == QLatin1String("--output-dir")) {
            if (args.count() == 0) {
                printUsage();
                return false;
            }
            if (!mOutputPath.isEmpty()) {
                return false;
            }
            mOutputPath = args.takeFirst();
        }
        else if (arg == QLatin1String("--input-dir")) {
            if (args.count() == 0) {
                printUsage();
                return false;
            }
            if (!mInputPath.isEmpty()) {
                return false;
            }
            mInputPath = args.takeFirst();
        }
        else if (arg == QLatin1String("--input")) {
            if (args.count() == 0) {
                printUsage();
                return false;
            }
            if (!mInputFile.isEmpty()) {
                return false;
            }
            mInputFile = args.takeFirst();
        }
        else if (arg == QLatin1String("--flow-file")) {
            if (args.count() == 0) {
                printUsage();
                return false;
            }
            mFlowFileName = args.takeFirst();
        }
        else if (arg == QLatin1String("--include-dir")) {
            if (args.count() == 0) {
                printUsage();
                return false;
            }
            mIncludePaths.append(args.takeFirst());
        }
        else if (arg == QLatin1String("--unit-dir")) {
            if (args.count() == 0) {
                printUsage();
                return false;
            }
            mUnitPaths.append(args.takeFirst());
        }
        else {
            printUsage();
            std::cout << "Unknown parameter: " << arg.toUtf8().constData() << "\n";
            return false;
        }
    }

    if (mInputFile.isEmpty() || mOutputPath.isEmpty()) {
        printUsage();
        return false;
    }

    return true;
}

void ProjectImpl::printUsage() {
    std::cout << "Pascal to C tanscoder\n\n"

                 "Usage:\n"
                 "\tpas2c <options>\n\n"

                 "Mandatory options:\n"
                 "\t--input file        Set the root file to <file>\n"
                 "\t--output-dir dir    Set the output directory to <dir>\n"

                 "\nAdditional options:\n"
                 "\t--input-dir dir     Search for input file in <dir>\n"
                 "\t--define def        Add <def> to the preprocessor defines\n"
                 "\t--include-dir dir   Add <dir> to the search paths for includes\n"
                 "\t--unit-dir dir      Add <dir> to the search paths for units\n"

                 "\nDebugging options:\n"
                 "\t--dump-models       Dump code models\n"
                 "\t--dump-streams      Dump token streams\n"
                 "\t--dump-stats        Dump file statistice\n"
                 "\t--dump-global       Dump global statistics\n"
                 "\t--dump-flow         Dump the parser flow\n"
                 "\t--flow-file file    Set output file for parser flow to <file>\n"
                 "\n";
}

bool ProjectImpl::initialize() {

    if (testOption(optDumpParserFlow)) {

        if (mFlowFileName.isEmpty()) {
            QDir dir(mOutputPath);
            QFileInfo fi(dir.filePath(mInputFile));
            mFlowFileName = dir.filePath(fi.completeBaseName() % QLatin1Literal(".uflow"));
        }

        QScopedPointer<QFile> flowFile(new QFile(mFlowFileName));
        if (!flowFile->open(QFile::WriteOnly)) {
            return false;
        }

        mFlowFile = flowFile.take();
        FlowTracer::instance().setOutput(mFlowFile);
        FlowTracer::instance().setEnabled(true);
    }

    return true;
}

Errors::Ptr ProjectImpl::errors() const {
    if (!mErrors) {
        Errors* e = new Errors;
        const_cast<ProjectImpl*>(this)->mErrors = e;
        return e;
    }

    return mErrors;
}

bool ProjectImpl::hasErrors() {

    if (!mErrors) {
        return false;
    }

    return errors()->hasErrors();
}

void ProjectImpl::dumpErrors() {
    errors()->dumpErrors();
}

bool ProjectImpl::run() {

    QString fName = locateSourceFile(mInputFile);

    if (mInputPath.isEmpty()) {
        QFileInfo fi(fName);
        mInputPath = fi.path();
        mInputFile = fi.fileName();
    }

    Model::Root::Ptr model = parseFile(fName);

    return true;
}

Model::Root::Ptr ProjectImpl::parseFile(const QString& fileName) {

    LexicalAnalyzer::Ptr la = analyze(fileName);
    if (!hasErrors()) {
        Q_ASSERT(la);

        Model::Root::Ptr model = parseStream(la->tokenStream());
        if (model) {

            if (testOption(optDumpCodeModel)) {
                QFileInfo fi(fileName);
                QString modelFileName =
                        fi.absolutePath() %
                        fi.completeBaseName() %
                        QLatin1Literal(".pmodel");

                if (!model->exportModel(modelFileName)) {
                    errors()->emitError("Could not export model for file.", fileName);
                }
            }

            return model;
        }

    }

    return NULL;
}

LexicalAnalyzer::Ptr ProjectImpl::analyze(const QString& fileName) {

    LexicalAnalyzer::Ptr la = newAnalyzer(fileName);

    if (la->analyze()) {

        if (testOption(optDumpTokenStreams)) {
            QFileInfo fi(fileName);
            QString tokFileName =
                    fi.absolutePath() %
                    fi.completeBaseName() %
                    QLatin1Literal(".ptokens");

            la->tokenStream()->dump(tokFileName);
        }

        return la;
    }

    if (!hasErrors()) {
        // In case the lexer did fail without setting an error, set one now
        errors()->emitError("Failed to analye source", fileName);
    }

    return NULL;
}

Model::Root::Ptr ProjectImpl::parseStream(TokenStream::Ptr stream) {
    FLOW_TRACER();

    Parsers::ContextStack::Ptr ctxStack = new Parsers::ContextStack;
    QScopedPointer<Parsers::Pascal> rootParser(Parsers::Pascal::create(ctxStack, this, stream));

    if (rootParser) {
        rootParser->setProject(this);

        if ((*rootParser)()) {
            return rootParser->model();
        }
    }

    if (!hasErrors()) {
        // In case, parser did not set an error, invent one
        errors()->emitError("Faild to parse file", QString());
    }
    return NULL;
}

QString ProjectImpl::locateFile(const QStringList& paths, const QStringList& exts,
                            const QString& file, bool tryBasenames) const {
    QFileInfo fi(file);

    foreach (const QString& path, paths) {

        if (path.isEmpty()) {
            continue;
        }

        QDir dir(path);

        if (QFile::exists(dir.filePath(file))) {
            return dir.filePath(file);
        }

        foreach (QString ext, exts) {

            if (ext.isEmpty()) {
                continue;
            }

            if (!ext.startsWith(QChar(L'.'))) {
                ext.prepend(QChar(L'.'));
            }

            QString fName = dir.filePath(fi.fileName() % ext);
            if (QFile::exists(fName)) {
                return fName;
            }

            if (tryBasenames) {
                dir.filePath(fi.completeBaseName() % ext);
                if (QFile::exists(fName)) {
                    return fName;
                }

                fName = dir.filePath(fi.baseName() % ext);
                if (QFile::exists(fName)) {
                    return fName;
                }
            }
        }
    }

    return QString();
}

QString ProjectImpl::locateSourceFile(const QString& fileNameFragment) const {

    QFileInfo fi(fileNameFragment);

    if (fi.isAbsolute()) {
        return fileNameFragment;
    }

    QStringList exts;
    exts << QLatin1String("pas")
         << QLatin1String("pp");

    QStringList searchPaths;
    searchPaths << mInputPath << mUnitPaths;

    return locateFile(searchPaths, exts, fileNameFragment, false);
}

InputStream::Ptr ProjectImpl::inputStreamFor(const QString& fileName) {
    return mInputStreams.streamFor(fileName);
}

void ProjectImpl::setFactory(Factories type, FactoryBase* factory) {
    mFactories[type] = factory;
}

FactoryBase* ProjectImpl::factory(Factories type) const {
    return mFactories.value(type, NULL);
}

void ProjectImpl::emitError(const QString& text, const InputStreamRef& ref) {
}

void ProjectImpl::emitError(const char* text, const InputStreamRef& ref) {
}

static inline Model::TypeStock* stockFromTokenType(TokenType type) {
    switch(type) {
    case T_STRING:      return new Model::TypeStock(Model::dtString);
    case T_INTEGER:     return new Model::TypeStock(Model::dtInteger);
    case T_FLOAT:       return new Model::TypeStock(Model::dtFloat);
    case T_BOOLEAN:     return new Model::TypeStock(Model::dtBoolean);
    case T_FILE:        return new Model::TypeStock(Model::dtFile);
    case T_POINTER:     return new Model::TypeStock(Model::dtPointer);
    case T_TEXT:        return new Model::TypeStock(Model::dtText);
    default:            return NULL;
    }
}

Model::TypeStock::Ptr ProjectImpl::stockType(TokenType tokenType) {
    Model::TypeStock::Ptr type = mStockTypes.value(tokenType, NULL);

    if (!type) {
        type = stockFromTokenType(tokenType);
        if (type) {
            mStockTypes.insert(tokenType, type);
        }
    }

    return type;
}
