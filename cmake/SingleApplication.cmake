option(FETCH_SINGLE_APPLICATION "Download SingleApplication" ON)

set(QAPPLICATION_CLASS QApplication)
set(QT_DEFAULT_MAJOR_VERSION 6)

if (FETCH_SINGLE_APPLICATION)
    CPMAddPackage(
        NAME SingleApplication
        GITHUB_REPOSITORY "itay-grudev/SingleApplication"
        GIT_TAG v3.2.0
        )
else()
    set(SINGLE_APPLICATION_DIR ${CMAKE_SOURCE_DIR}/3rdpart/SingleApplication)

    set(SINGLE_APPLICATION_INCLUDE_PATH ${SINGLE_APPLICATION_DIR})

    set(SINGLE_APPLICATION_LIBRARY SingleApplication)

    add_library(${SINGLE_APPLICATION_LIBRARY} STATIC
        ${SINGLE_APPLICATION_INCLUDE_PATH}/singleapplication.h
        )

    target_include_directories(${SINGLE_APPLICATION_LIBRARY} INTERFACE
        ${SINGLE_APPLICATION_INCLUDE_PATH}
        )
endif()
