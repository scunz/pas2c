
PROJECT(PAS2C_LEX_ANAL)

QT_PREPARE(Core -Gui)

SET(SRC_FILES
    TokenImpl.cpp
    TokenStreamImpl.cpp
    LexicalAnalysis.cpp
    LexicalAnalysisImpl.cpp
    LexerFactory.cpp
)

SET(HDR_FILES
    LexAnalAPI.hpp
    TokenImpl.hpp
    TokenStreamImpl.hpp
    LexicalAnalysisImpl.hpp
    LexerFactory.hpp
)

INCLUDE_DIRECTORIES(
    ${PAS2C_CORE_UTILS_SOURCE_DIR}
    ${PAS2C_INTERFACES_SOURCE_DIR}
    ${PAS2C_LEX_ANAL_SOURCE_DIR}
)

ADD_QT_LIBRARY(
    pascLexAnal SHARED

    ${SRC_FILES}
    ${HDR_FILES}
)
