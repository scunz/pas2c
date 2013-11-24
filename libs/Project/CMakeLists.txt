
PROJECT(PAS2C_PROJECT)

QT_PREPARE(Core -Gui)

INCLUDE(files.cmake)
SET(SRC_FILES ${_SRC_FILES})
SET(HDR_FILES ${_HDR_FILES})

INCLUDE_DIRECTORIES(
    ${PAS2C_CORE_UTILS_SOURCE_DIR}
    ${PAS2C_CODE_MODEL_SOURCE_DIR}
    ${PAS2C_INTERFACES_SOURCE_DIR}
    ${PAS2C_LEX_ANAL_SOURCE_DIR}
    ${PAS2C_PARSERS_SOURCE_DIR}
)

ADD_QT_LIBRARY(
    pascProject ${LIB_SCOPE}

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
