
#ifndef PASC_ERRORS_HPP
#define PASC_ERRORS_HPP

#include <QString>

#include "common/ptr.hpp"

class Errors : public PtrBase
{
public:
    typedef PtrT<Errors> Ptr;
public:
    void emitError(const QString& text, const QString& fileName, int line = -1, int column = -1);
};

#endif
