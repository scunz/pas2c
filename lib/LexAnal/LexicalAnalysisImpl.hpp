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

#ifndef PASC_LEXANAL_IMPL_HPP
#define PASC_LEXANAL_IMPL_HPP

// CoreUtils
#include "InputStream.hpp"

// Interfaces
#include "Project.hpp"
#include "LexicalAnalysis.hpp"

// LexAnal
#include "TokenStreamImpl.hpp"

#ifndef PASC_LEX_ANAL_NO_INTERNALS

class LexicalAnalyzerImpl : public LexicalAnalyzer {
public:
    LexicalAnalyzerImpl();

public:
    void setProject(Project::Ptr prj);
    void setInput(const QString& fileName);
    TokenStream::Ptr tokenStream() const;

private:
    static TokenType isOperToken(const char* token);
    static TokenType isTextToken(const char* token);

    bool analyze();
    void skipSpace();
    bool eof();
    bool skipBraceComment(QByteArray& out);
    bool skipParenComment(QByteArray& out);
    bool preprocessor(const QByteArray& comment);
    void emitError(const QString& text, const InputStreamRef& ref);
    void emitError(const char* text, const InputStreamRef& ref);
    InputStreamRef mkRef(int c) { return InputStreamRef(mState->mStream, mState->mLine, c); }

private:
    TokenStreamImpl::Ptr mTokenStream;
    Project::Ptr mProject;
    QString mFileName;

    struct State {
        State(const InputStream::Ptr& inStream, State* prev = NULL)
            : mStream(inStream)
            , mPos(0)
            , mLine(-1)
            , mColumn(-1)
            , mPrev(prev)
        {
        }

        char ch() const { return mStream->at(mPos); }
        char chNext() const {
            if (mPos + 1 < mStream->size()) return (char)0;
            return mStream->at(mPos+1);
        }

        InputStream::Ptr mStream;
        int mPos;
        int mLine;
        int mColumn;
        State* mPrev;
    };
    State* mState;
    bool mIsAnalyed;
};

#endif // PASC_LEX_ANAL_NO_INTERNALS

#endif
