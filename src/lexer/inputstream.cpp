
#include <QStringBuilder>
#include <QFile>

#include "lexer/inputstream.hpp"

InputStream::InputStream(Errors::Ptr errors, const QString& fileName)
    : mFileName(fileName)
    , mErrors(errors)
{
    QFile f(mFileName);
    if (!f.open(QFile::ReadOnly)) {
        mErrors->emitError(QLatin1String("Cannot read input file."), fileName);
        return;
    }
    mData = f.readAll();
}

