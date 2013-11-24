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

#include "FlowTracer.hpp"

FlowTracer::FlowTracer()
    : mOutput(NULL)
    , mLevel(-1)
    , mEnabled(false)
{
    Q_ASSERT(sInstance == NULL);
    sInstance = this;
}

FlowTracer::~FlowTracer() {
    sInstance = NULL;
}

FlowTracer* FlowTracer::sInstance = NULL;

void FlowTracer::setOutput(QFile* output) {
    mOutput = output;
}

void FlowTracer::setEnabled(bool enabled) {
    mEnabled = enabled;
}

FlowTracer& FlowTracer::instance() {
    Q_ASSERT(sInstance);
    return *sInstance;
}

void FlowTracer::enter(Flow* flow) {
    mLevel++;
    if (mEnabled) {
        QByteArray a = QByteArray(mLevel, ' ') % flow->mFuncInfo;
        if (!flow->mExtraText.isEmpty()) {
            int align = mOutput ? 140 : 80;
            if (a.length() < align) {
                a = a % QByteArray(align - a.length(),' ');
            }
            a = a % " (" % flow->mExtraText % ")";
        }
        if (mOutput) {
            a = a % "\n";
            mOutput->write(a);
        }
        else {
            qDebug("%s", a.constData());
        }
    }
}

void FlowTracer::leave(Flow* flow) {
    mLevel--;
}

Flow::Flow(const QByteArray& funcInfo, const QByteArray&extraText)
    : mFuncInfo(funcInfo)
    , mExtraText(extraText)
{
    FlowTracer::instance().enter(this);
}

Flow::~Flow() {
    FlowTracer::instance().leave(this);
}
