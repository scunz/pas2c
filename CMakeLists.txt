
PROJECT( Pas2CTranscoder )
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

INCLUDE(cmake/Utils.cmake)
SET_OUTPUT_PATHS()
INCLUDE(cmake/QtMacros.cmake)

IF(UNIX)
    GCC_ADD_FLAG(-fvisibility=hidden)
    GCC_ADD_FLAG(-fvisibility-inlines-hidden)
ENDIF()

ADD_SUBDIRECTORY(libs)
ADD_SUBDIRECTORY(src)
