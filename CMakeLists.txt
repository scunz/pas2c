
PROJECT( Pas2CTranscoder )
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

INCLUDE(cmake/Utils.cmake)
SET_OUTPUT_PATHS()
INCLUDE(cmake/QtMacros.cmake)

IF(UNIX)
    GCC_ADD_FLAG(-fvisibility=hidden)
    GCC_ADD_FLAG(-fvisibility-inlines-hidden)
ENDIF()

# These are the 3 different modes of compiling a project like this:

# First "all-in-one-lib". Suitable for developing modules upon something (pasc doesn't support that
# but it's still nice). Compiles all files into one shared lib and uses a stub to invoke it. All
# public symbols exported from the lib (i.e. works on Windows, too). Highest linktime.

# "Monolithic": Nothing exported. Everything into a single binary. Most portable, but also has high
# link time. Good suited for deployment.

# "All Shared": Create a shared library for each component. Most reduced link time, but cumbersome
# to distribute. Suited very well for development.

# The cmake files here, demonstrate how to implement all of those 3 in the same source tree. Using
# a file called "files.cmake" in each directory the same way, qmake would use a .pri file.

IF(1)   # Build shared library + 1 consumer
    SET(BUILD_MODE "All-in-one-lib")
    SET(LIB_SCOPE SHARED)
    ADD_DEFINITIONS(-DBUILD_ALL_IN_ONE_LIB)
ENDIF()

IF(0)   # Build monolithic app
    SET(BUILD_MODE "Static")
    SET(LIB_SCOPE STATIC)
    ADD_DEFINITIONS(-DBUILD_ALL_IN_ONE_LIB)
    ADD_DEFINITIONS(-DBUILD_STATIC)
ENDIF()

IF(0)   # Build Shared Libraries
    SET(BUILD_MODE "Shared")
    SET(LIB_SCOPE SHARED)
    ADD_DEFINITIONS(-DBUILD_SHARED)
ENDIF()

ADD_SUBDIRECTORY(lib)
ADD_SUBDIRECTORY(src)
