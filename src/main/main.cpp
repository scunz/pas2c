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

#include <QCoreApplication>

#include "FlowTracer.hpp"

#include "ProjectImpl.hpp"

int main(int c, char** v) {

    QCoreApplication app(c, v);
    FlowTracer ft;

    QStringList args;
    args << QLatin1String("--output-dir")       << QLatin1String("/work/pastest/bin")
         << QLatin1String("--input-dir")        << QLatin1String("/work/pastest/src")
         << QLatin1String("--input")            << QLatin1String("file1")
         << QLatin1String("--include-dir")      << QLatin1String("/work/pastest/src")
         << QLatin1String("--include-dir")      << QLatin1String("/work/pastest/src/inc")
         << QLatin1String("--unit-dir")         << QLatin1String("/work/pastest/src")
         << QLatin1String("--unit-dir")         << QLatin1String("/work/pastest/bin")
    //   << QLatin1String("--mode")             << QLatin1String("ancient")

         << QLatin1String("--dump-flow")
         << QLatin1String("--flow-file")        << QLatin1String("pasc.uflow")
         << QLatin1String("--dump-models")
         << QLatin1String("--dump-streams")
         << QLatin1String("--dump-stats")
         << QLatin1String("--dump-global");

    ProjectImpl::Ptr prj = new ProjectImpl;
    if (!prj->parseArguments(args)) {
        return 1;
    }

    if (!prj->initialize()) {
        return 1;
    }

    if (prj->run()) {
        return 0;
    }

    prj->dumpErrors();
    return 1;
}
