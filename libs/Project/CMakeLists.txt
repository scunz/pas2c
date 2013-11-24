
PROJECT(PAS2C_PROJECT)

QT_PREPARE(Core -Gui)

SET(SRC_FILES
    ProjectImpl.cpp
)

SET(HDR_FILES
    ProjectImpl.hpp
)

INCLUDE_DIRECTORIES(
    ${PAS2C_CORE_UTILS_SOURCE_DIR}
    ${PAS2C_CODE_MODEL_SOURCE_DIR}
    ${PAS2C_INTERFACES_SOURCE_DIR}
    ${PAS2C_LEX_ANAL_SOURCE_DIR}
    ${PAS2C_PARSERS_SOURCE_DIR}
)

ADD_QT_LIBRARY(
    pascProject SHARED

    ${SRC_FILES}
    ${HDR_FILES}
)

TARGET_LINK_LIBRARIES(
    pascProject

    LINK_PRIVATE
        pascInterfaces
        pascParsers
        pascCodeModel
        pascLexAnal
        pascCoreUtils
)
