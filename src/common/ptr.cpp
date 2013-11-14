
#include "common/ptr.hpp"

PtrBase::PtrBase()
    : mRefCount(0)
{
}

PtrBase::~PtrBase() {
}

void PtrBase::addRef() {
    mRefCount++;
}

void PtrBase::deRef() {
    if (!--mRefCount) {
        delete this;
    }
}
