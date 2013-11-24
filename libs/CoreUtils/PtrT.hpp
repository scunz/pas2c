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

#ifndef PASC_PTRT_HPP
#define PASC_PTRT_HPP

template< class T >
class PtrT {
public:
    PtrT() : d(0) { addRef(); }
    PtrT(const PtrT& other) : d(other.d) { addRef(); }
    PtrT(T* t) : d(t) { addRef(); }

    template< class X >
    PtrT(X* x) : d(x) { addRef(); }

    template< class X >
    PtrT(PtrT<X> other) : d(other.data()) { addRef(); }

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
    PtrT& operator=(PtrT<X> other ) {
        if (d != other.data() ) {
            deRef();
            d = other.data();
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

    template<class X>
    X* scast() {
        return static_cast<X*>(d);
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

#endif
