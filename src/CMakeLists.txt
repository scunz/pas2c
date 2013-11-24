
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


IF(${BUILD_MODE} STREQUAL "All-in-one-lib")

    INCLUDE_DIRECTORIES(
        ${PAS2C_LIBRARY_SOURCE_DIRS}
    )

ELSE()
    INCLUDE_DIRECTORIES(
        ${PAS2C_CORE_UTILS_SOURCE_DIR}
        ${PAS2C_INTERFACES_SOURCE_DIR}
        ${PAS2C_CODE_MODEL_SOURCE_DIR}
        ${PAS2C_PROJECT_SOURCE_DIR}
    )
ENDIF()

ADD_QT_EXECUTABLE(
    pas2c

    ${SRC_FILES}
    ${HDR_FILES}
)

IF(${BUILD_MODE} STREQUAL "All-in-one-lib")
    TARGET_LINK_LIBRARIES(
        pas2c
        LINK_PRIVATE
            pascLibrary
    )
ELSE()
    TARGET_LINK_LIBRARIES(
        pas2c

        LINK_PRIVATE
            pascParsers
            pascCodeModel
            pascProject
            pascLexAnal
            pascCoreUtils
    )
ENDIF()
