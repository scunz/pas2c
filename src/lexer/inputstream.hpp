
#ifndef PASC_INPUT_STREAM_HPP
#define PASC_INPUT_STREAM_HPP

#include <QString>

#include "common/ptr.hpp"
#include "common/errors.hpp"

#include "lexer/token.hpp"

class InputStream : public PtrBase
{
public:
    typedef PtrT<InputStream> Ptr;
public:
    InputStream(Errors::Ptr errors, const QString& fileName);

public:
    QString fileName() const { return mFileName; }
    QByteArray data() const { return mData; }

private:
    Errors::Ptr mErrors;
    QString mFileName;
    QByteArray mData;
    Tokens mTokens;
};

#endif
