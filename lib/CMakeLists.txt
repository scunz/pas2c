
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

    SET(PAS2C_LIBRARY_SOURCE_DIRS)

    FOREACH(DIR ${DIRS})

        SET(_HDR_FILES)
        SET(_SRC_FILES)
        INCLUDE(${DIR}/files.cmake)


        SET(PAS2C_LIBRARY_SOURCE_DIRS
            ${PAS2C_LIBRARY_SOURCE_DIRS}
            ${CMAKE_CURRENT_LIST_DIR}/${DIR}
        )

        FOREACH(FILE ${_SRC_FILES})
            SET(SRC_FILES ${SRC_FILES} ${DIR}/${FILE})
        ENDFOREACH()

        FOREACH(FILE ${_HDR_FILES})
            SET(HDR_FILES ${HDR_FILES} ${DIR}/${FILE})
        ENDFOREACH()

    ENDFOREACH()

    SET(PAS2C_LIBRARY_SOURCE_DIRS ${PAS2C_LIBRARY_SOURCE_DIRS}
        PARENT_SCOPE)
    INCLUDE_DIRECTORIES(${PAS2C_LIBRARY_SOURCE_DIRS})

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
