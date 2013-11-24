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

#include "ExportInfo.hpp"
#include "Model/Type/Stock.hpp"

namespace Model
{

    void TypeStock::exportModel(ExportInfo& ei, QDomElement &elParent) const {

        QString typeName;

        switch(mType) {
        case dtString:  typeName = QLatin1String("string");     break;
        case dtBoolean: typeName = QLatin1String("boolean");    break;
        case dtFloat:   typeName = QLatin1String("float");      break;
        case dtInteger: typeName = QLatin1String("integer");    break;
        case dtPointer: typeName = QLatin1String("pointer");    break;
        case dtChar:    typeName = QLatin1String("char");       break;
        default:        Q_ASSERT(false);
        }

        QDomElement elMe;
        if (ei.wantExport(this, elParent, elMe)) {
            ei.newTextElement(elMe, "Type.Stock.Id", typeName.toUtf8().constData());
        }
    }

    void TypeStock::importModel(QDomElement &el) {
        Q_ASSERT(el.tagName() == QLatin1String("StockType"));

        QString txt = el.text();
        if (txt == QLatin1String("string")) {
            mType = dtString;
        }
        else if(txt == QLatin1String("boolean")) {
            mType = dtBoolean;
        }
        else if(txt == QLatin1String("float")) {
            mType = dtFloat;
        }
        else if(txt == QLatin1String("integer")) {
            mType = dtInteger;
        }
        else if(txt == QLatin1String("pointer")) {
            mType = dtPointer;
        }
        else if(txt == QLatin1String("char")) {
            mType = dtChar;
        }
        else {
            Q_ASSERT(false);
        }
    }

    const char* TypeStock::className() const {
        return "Type.Stock";
    }

    Objects TypeStock::type() const {
        return oTypeStock;
    }

    #if 0 // Violates Architecture
    TypeStock* TypeStock::fromTokenType(TokenType type) {
        switch(type) {
            case T_STRING:      return new TypeStock(dtString);
            case T_INTEGER:     return new TypeStock(dtInteger);
            case T_FLOAT:       return new TypeStock(dtFloat);
            case T_BOOLEAN:     return new TypeStock(dtBoolean);
            case T_FILE:        return new TypeStock(dtFile);
            case T_POINTER:     return new TypeStock(dtPointer);
            case T_TEXT:        return new TypeStock(dtText);
            default:            return NULL;
        }
    }
    #endif

    DataTypes TypeStock::dataType() const {
        return mType;
    }

    void TypeStock::setDataType(DataTypes t) {
        mType = t;
    }

}
