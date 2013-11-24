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

#ifndef PASC_FLOW_TRACER_HPP
#define PASC_FLOW_TRACER_HPP

#include <QFile>

#include "CoreUtilsAPI.hpp"

class FlowTracer;

class PASC_CORE_UTILS_API Flow {
    friend class FlowTracer;
public:
    Flow(const QByteArray& funcInfo, const QByteArray& extraText);
    ~Flow();

private:
    QByteArray mFuncInfo;
    QByteArray mExtraText;
};

class PASC_CORE_UTILS_API FlowTracer
{
public:
    FlowTracer();
    ~FlowTracer();

public:
    void setOutput(QFile* output);
    void setEnabled(bool enable);
    void enter(Flow* flow);
    void leave(Flow* flow);

public:
    static FlowTracer& instance();

private:
    static FlowTracer* sInstance;
    QFile* mOutput;
    int mLevel;
    bool mEnabled;
};

#define FLOW_TRACER() \
    Flow flow(Q_FUNC_INFO, NULL)

#define FLOW_TRACER_EX(ex) \
    Flow flow(Q_FUNC_INFO, ex)

#endif
