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

#include "Base.hpp"

namespace Parsers {

    Base::Base(const ContextStack::Ptr& ctxStack,
               const Errors::Ptr& errs,
               const TokenStream::Ptr& stream)
        : mCtxStack(ctxStack)
        , mErrors(errs)
        , mStream(stream)
    {
    }

    void Base::emitError(const QString& text) {
        emitError(text, current().inputStreamRef());
    }

    void Base::emitError(const QString& text, const InputStreamRef& ref) {
        ref.emitError(mErrors, text);
    }

    void Base::emitError(const char* text) {
        emitError(text, current().inputStreamRef());
    }

    void Base::emitError(const char* text, const InputStreamRef& ref) {
        ref.emitError(mErrors, text);
    }

}
