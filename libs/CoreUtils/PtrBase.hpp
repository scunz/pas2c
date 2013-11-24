
#ifndef PASC_PTR_BASE_HPP
#define PASC_PTR_BASE_HPP

#include "CoreUtilsAPI.hpp"
#include "PtrT.hpp"

class PASC_CORE_UTILS_API PtrBase {
protected:
    PtrBase();
    virtual ~PtrBase();

public:
    void addRef();
    void deRef();

private:
    PtrBase(const PtrBase& other);
    PtrBase& operator=(const PtrBase&);

    int mRefCount;  // ### Use QAtomicInt
};

#endif
