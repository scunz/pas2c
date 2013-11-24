
PROJECT(pasc)

QT_PREPARE(-Gui Xml)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

SET(SRC_FILES
    main.cpp
)

SET(HDR_FILES
)

INCLUDE_DIRECTORIES(
    ${PAS2C_CORE_UTILS_SOURCE_DIR}
    ${PAS2C_INTERFACES_SOURCE_DIR}
    ${PAS2C_CODE_MODEL_SOURCE_DIR}
    ${PAS2C_PROJECT_SOURCE_DIR}
)

ADD_QT_EXECUTABLE(
    pas2c

    ${SRC_FILES}
    ${HDR_FILES}
)

TARGET_LINK_LIBRARIES(
    pas2c

    LINK_PRIVATE
        pascParsers
        pascCodeModel
        pascProject
        pascLexAnal
        pascCoreUtils
)
