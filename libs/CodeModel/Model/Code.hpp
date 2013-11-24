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

#ifndef PASC_CODE_MODEL_HPP
#define PASC_CODE_MODEL_HPP

#include "PtrBase.hpp"

#include "Identifier.hpp"

class QDomDocument;
class QDomElement;

namespace Model {

    class ExportInfo; // Internal

    enum Objects {
        oProgram,
        oUnit,

        oDeclList,

        oDeclVar,
        oDeclType,      // unsupported
        oDeclConst,     // unsupported
        oDeclProc,
        oDeclFunc,      // unsupported
        oDeclUses,      // unsupported

        oTypeStock,
        oTypeEnum,
        oTypeCustom,
        oTypeCustomAhead,   // Helper, not really used in final model!
        oTypePointerOf,
        oTypeArrayOf,   // unsupported
        oTypeRecordOf,  // unsupported
        oTypeRecordField,
        oTypeSetOf,
        oTypeRange,     // unsupported
        oTypeFileOf,

        oLValuePointerDeref,
        oLValueRecordDeref,
        oLValueSubscriptDeref,
        oLValueTypeCast,
        oLValueUnitDeref,
        oLValueVarRef,

        oStmtCompound,
        oStmtList,
        oStmtIf,
        oStmtWhile,
        oStmtRepeat,
        oStmtCase,
        oStmtWith,
        oStmtAssign,
        oStmtCall,

        oExprUnary,
        oExprBinary,

        oExprLValue,

        oExprConstNumber,
        oExprConstDouble,
        oExprConstString,
        oExprConstConst,

        oInvalid
    };

    enum UnaryOperator {
        operNoneUnary,

        operNot,
        operAddrOf,
        operKeepSign,
        operToggleSign
    };

    enum BinaryOperator {
        operNoneBinary,

        operAnd,
        operOr,
        operXor,
        operLess,
        operLessEqual,
        operGreater,
        operGreaterEqual,
        operEqual,
        operInequal,
        operIn,
        operAdd,
        operMinus,
        operMult,
        operDivision,
        operIntDiv,
        operIntMod,
        operShiftLeft,
        operShiftRight
    };

    enum DataTypes {
        dtUnknown = -1,
        dtInteger = 0,
        dtString,
        dtFloat,
        dtBoolean,
        dtPointer,
        dtChar,
        dtFile,
        dtText
        // more...
    };

    class Code : public PtrBase
    {
    public:
        typedef PtrT<Code> Ptr;

    public:
        Code();

    public:
        virtual void exportModel(ExportInfo& ei, QDomElement& elParent) const = 0;
        virtual const char* className() const = 0;
        virtual Objects type() const = 0;

    protected: // tools
        static void exportIdentifier(QDomElement& elParent, const Identifier& name);
    };

}

#endif
