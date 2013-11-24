SET(_SRC_FILES
    Identifier.cpp
    ExportInfo.cpp
)

SET(_HDR_FILES
    CodeModelAPI.hpp
    Identifier.hpp
    ExportInfo.hpp
)

SET(models
    Code

    Root
    Root/Program
    Root/Unit

    Expr
    Expr/Unary
    Expr/Binary
    Expr/Const
    Expr/Const/Const
    Expr/Const/Float
    Expr/Const/Number
    Expr/Const/String
    Expr/Const/Set
    Expr/Const/Record
    Expr/Const/Array
    Expr/LValue

    Decl
    Decl/List
    Decl/Var
    Decl/Routine
    Decl/Routine/Params
    Decl/Func
    Decl/Proc
    Decl/Const
    Decl/Type

    Type
    Type/Stock
    Type/Of
    Type/File
    Type/Pointer
    Type/Enum
    Type/Range
    Type/Array
    Type/Set
    Type/Record
    Type/RecordField
    Type/Custom
    Type/CustomAhead        # Helper Model entity, should never really exist in a fully parsed model

    Stmt
    Stmt/List
    Stmt/Compound
    Stmt/If
    Stmt/While
    Stmt/Repeat
    Stmt/For
    Stmt/Assign
    Stmt/Call
    Stmt/With

    LValue
    LValue/VarRef
    LValue/TypeCast
    LValue/RecordDeref
    LValue/PointerDeref
    LValue/SubscriptDeref
    LValue/UnitDeref
    # with classes we'll need a func-call-deref here too
)

FOREACH(model ${models})
    SET(_SRC_FILES ${_SRC_FILES} Model/${model}.cpp)
    SET(_HDR_FILES ${_HDR_FILES} Model/${model}.hpp)
ENDFOREACH()
