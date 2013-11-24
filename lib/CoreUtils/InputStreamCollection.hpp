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

#ifndef PASC_INPUT_STREAM_COLLECTION_HPP
#define PASC_INPUT_STREAM_COLLECTION_HPP

#include <QHash>

// CoreUtils
#include "PtrBase.hpp"
#include "InputStream.hpp"

class PASC_CORE_UTILS_API InputStreamCollection : public PtrBase {
public:
    bool hasStreamFor(const QString& fileName) const;
    void insert(InputStream::Ptr stream);
    InputStream::Ptr streamFor(const QString& fileName);

private:
    QHash<QString,
          InputStream::Ptr> mStreams;
};

#endif
