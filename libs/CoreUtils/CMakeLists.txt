
PROJECT(PAS2C_CORE_UTILS)

QT_PREPARE(Core -Gui)

SET(SRC_FILES
    PtrBase.cpp
    Error.cpp
    Errors.cpp
    FlowTracer.cpp
    InputStream.cpp
    InputStreamRef.cpp
    InputStreamCollection.cpp
)

SET(HDR_FILES
    CoreUtilsAPI.hpp
    PtrBase.hpp
    Error.hpp
    Errors.hpp
    FlowTracer.hpp
    InputStream.hpp
    InputStreamRef.hpp
    InputStreamCollection.hpp
    PtrT.hpp
)

ADD_QT_LIBRARY(
    pascCoreUtils SHARED

    ${SRC_FILES}
    ${HDR_FILES}
)
