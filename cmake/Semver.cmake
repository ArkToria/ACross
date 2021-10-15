option(FETCH_SEMVER "Download Semver" ON)

if(FETCH_SEMVER)
    CPMAddPackage(
        NAME semver
        GITHUB_REPOSITORY Neargye/semver
        GIT_TAG v0.3.0
        )
else()
    set(SEMVER_DIR ${CMAKE_SOURCE_DIR}/3rdpart/semver)

    set(SEMVER_INCLUDE_PATH ${SEMVER_DIR}/include)

    set(SEMVER_LIBRARY semver)

    add_library(${SEMVER_LIBRARY} STATIC
        ${SEMVER_INCLUDE_PATH}/semver.hpp
        )

    target_include_directories(${SEMVER_LIBRARY} INTERFACE
        ${SEMVER_INCLUDE_PATH}
        )
endif()
