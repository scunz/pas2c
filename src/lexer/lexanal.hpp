
#ifndef PASC_LEXANAL_HPP
#define PASC_LEXANAL_HPP

#include "common/errors.hpp"

#include "lexer/inputstream.hpp"

class LexicalAnalyzer : public PtrBase
{
public:
    typedef PtrT<LexicalAnalyzer> Ptr;

public:
    LexicalAnalyzer(Errors::Ptr errors, const QString& fileName);

public:
    bool analyze();
    Tokens tokens();

private:
    Errors::Ptr mErrors;
    InputStream::Ptr mInputStream;
    bool mIsAnalyed;
    Tokens mTokens;
};

#endif
