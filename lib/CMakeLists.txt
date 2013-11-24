
SET(DIRS

    CoreUtils
    CodeModel
    Interfaces
    LexAnal
    Parsers
    Project
)

IF(${BUILD_MODE} STREQUAL "All-in-one-lib")

    QT_PREPARE(Core -Gui Xml)
    ADD_DEFINITIONS(-DBUILD_SINGLE_LIB)

    FOREACH(DIR ${DIRS})

        SET(_HDR_FILES)
        SET(_SRC_FILES)
        INCLUDE(${DIR}/files.cmake)

        INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/${DIR})

        FOREACH(FILE ${_SRC_FILES})
            SET(SRC_FILES ${SRC_FILES} ${DIR}/${FILE})
        ENDFOREACH()

        FOREACH(FILE ${_HDR_FILES})
            SET(HDR_FILES ${HDR_FILES} ${DIR}/${FILE})
        ENDFOREACH()

    ENDFOREACH()

    ADD_QT_LIBRARY(
        pascLibrary SHARED

        ${SRC_FILES}
        ${HDR_FILES}
    )

ELSE()
    FOREACH(DIR ${DIRS})
        ADD_SUBDIRECTORY(${DIR})
    ENDFOREACH()
ENDIF()
