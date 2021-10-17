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
    add_subdirectory(${CMAKE_SOURCE_DIR}/3rdpart/SingleApplication)
endif()
