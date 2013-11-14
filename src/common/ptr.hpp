
#ifndef PASC_PTR_HPP
#define PASC_PTR_HPP

template< class T >
class PtrT {
public:
    PtrT() : d(0) { addRef(); }
    PtrT(const PtrT& other) : d(other.d) { addRef(); }
    PtrT(T* t) : d(t) { addRef(); }

    template< class X >
    PtrT(X* x) : d(x) { addRef(); }

    template< class X >
    PtrT(const PtrT<X>& other) : d(other.d) { addRef(); }

    ~PtrT() { deRef(); }

public:
    T* data() { return d; }
    const T* data() const { return d; }

    operator const T*() const { return d; }
    operator T*() { return d; }

    const T* operator->() const { return d; }
    T* operator->() { return d; }

    bool operator==(const PtrT& other) { return d == other.d; }
    bool operator!=(const PtrT& other) { return d != other.d; }
    bool operator!() const { return !d; }

    PtrT& operator=(const PtrT& other ) {
        if (d != other.d ) {
            deRef();
            d = other.d;
            addRef();
        }
        return *this;
    }

    PtrT& operator=(T* other ) {
        if (d != other ) {
            deRef();
            d = other;
            addRef();
        }
        return *this;
    }

    template< class X >
    PtrT& operator=(const PtrT<X>& other ) {
        if (d != other.d ) {
            deRef();
            d = other.d;
            addRef();
        }
        return *this;
    }

    template< class X >
    PtrT& operator=(X* other ) {
        if (d != other ) {
            deRef();
            d = other;
            addRef();
        }
        return *this;
    }

private:
    void addRef()
    {
        if (d) {
            d->addRef();
        }
    }

    void deRef() {
        if (d) {
            d->deRef();
        }
    }

    T* d;
};

class PtrBase {
protected:
    PtrBase();
    virtual ~PtrBase();

public:
    void addRef();
    void deRef();

private:
    PtrBase(const PtrBase& other);
    PtrBase& operator=(const PtrBase&);

    int mRefCount;
};

#endif
