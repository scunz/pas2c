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

#include <QFile>

#include "TokenStreamImpl.hpp"

TokenStreamImpl::TokenStreamImpl()
    : mPos(-2)
{
}

TokenStreamImpl::~TokenStreamImpl()
{
}


void TokenStreamImpl::appendTokens(const QVector<TokenImpl>& tokens)
{
    // we're creating a copy of each token here.

    // This creates deep copies of all T__STRING and T__IDENTIFIER tokens. We need a string list to
    // get rid of that... But that's an improvement for the future...

    foreach (const TokenImpl& token, tokens) {
        appendToken(token);
    }
}

void TokenStreamImpl::appendToken(const TokenImpl& token) {

    // This paranoia will produce some slow motion ;-)
    InputStream::Ptr is = token.inputStreamRef().stream();
    if (is) {
        QString fileName = is->fileName();
        if (!mInputStreams.hasStreamFor(fileName)) {
            qDebug() << "Adding token to token stream that refers an input stream not added to the "
                        "token stream. IS=" << is->fileName();
            appendInputStream(is);
        }
    }

    mTokens.append(token);
}

bool TokenStreamImpl::finalize() {
    if (mPos != -2) {
        return false;
    }

    mPos = -1;
    return true;
}

void TokenStreamImpl::appendInputStream(InputStream::Ptr stream) {
    Q_ASSERT(stream);

    if (!mInputStreams.hasStreamFor(stream->fileName())) {
        mInputStreams.insert(stream);
    }
}

void TokenStreamImpl::dump(const QString& fileName) const {
    QFile fTokens(fileName);
    fTokens.open(QFile::WriteOnly);
    QTextStream sTokens(&fTokens);

    foreach (const TokenImpl& t, mTokens) {
        sTokens << t.description();
        sTokens << "\n";
    }
}

const Token& TokenStreamImpl::at(int pos) const {
    Q_ASSERT(mPos != -2);

    if (pos >= 0 && pos < mTokens.count()) {
        return mTokens.at(pos);
    }
    else {
        static TokenImpl tEOS(InputStreamRef(), T__END_OF_STREAM);
        return tEOS;
    }
}

const Token& TokenStreamImpl::current() const {
    return at(mPos);
}

bool TokenStreamImpl::advance() const {
    return ++mPos != mTokens.count();
}

const Token& TokenStreamImpl::next() const {
    advance();
    return current();
}

const Token& TokenStreamImpl::ahead(int count) const {
    return at(mPos + count);
}

bool TokenStreamImpl::isAhead(TokenType type, int count) const {
    return ahead(count).type() == type;
}

bool TokenStreamImpl::require(TokenType type) const {
    return next().type() == type;
}
