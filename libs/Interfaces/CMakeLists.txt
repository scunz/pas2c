
PROJECT(PAS2C_INTERFACES)

QT_PREPARE(Core -Gui)

SET(SRC_FILES
    LexicalAnalysis.cpp
    Project.cpp
    Token.cpp
    TokenStream.cpp
    Createable.cpp
)

SET(HDR_FILES
    InterfacesAPI.hpp
    Project.hpp
    Token.hpp
    LexicalAnalysis.hpp
    TokenStream.hpp
    TokenTypes.hpp
    Createable.hpp
)

INCLUDE_DIRECTORIES(
    ${PAS2C_CORE_UTILS_SOURCE_DIR}
)

ADD_QT_LIBRARY(
    pascInterfaces SHARED

    ${SRC_FILES}
    ${HDR_FILES}
)

TARGET_LINK_LIBRARIES(
    pascInterfaces

    LINK_PRIVATE
        pascCoreUtils
)
