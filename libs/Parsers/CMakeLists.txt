
PROJECT(PAS2CS)

QT_PREPARE(-Gui Xml)

INCLUDE_DIRECTORIES(
    ${PAS2C_CORE_UTILS_SOURCE_DIR}
    ${PAS2C_CODE_MODEL_SOURCE_DIR}
    ${PAS2C_INTERFACES_SOURCE_DIR}
)

SET(SRC_FILES
    Context.cpp

    Base.cpp
    Pascal.cpp
    Unit.cpp
    Program.cpp
    DeclList.cpp
    Type.cpp
    IdentifierList.cpp
    StmtList.cpp
    Stmt.cpp
    Expr.cpp
    LValue.cpp
)

SET(HDR_FILES
    ParsersAPI.hpp

    Context.hpp

    Base.hpp
    Pascal.hpp
    Unit.hpp
    Program.hpp
    DeclList.hpp
    Type.hpp
    IdentifierList.hpp
    StmtList.hpp
    Stmt.hpp
    Expr.hpp
    LValue.hpp
)

ADD_QT_LIBRARY(
    pascParsers SHARED

    ${SRC_FILES}
    ${HDR_FILES}
)
