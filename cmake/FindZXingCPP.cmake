option(FETCH_ZXING_CPP "Download zxing-cpp" OFF)

if (FETCH_ZXING_CPP)
    add_definitions(-DUSE_FETCH_ZXING_CPP)
    message("Fetching 'zxing-cpp'...")

    set(BUILD_EXAMPLES OFF CACHE STRING "" FORCE)
    set(BUILD_BLACKBOX_TESTS OFF CACHE STRING "" FORCE)

    CPMAddPackage(
        NAME zxing-cpp
        GITHUB_REPOSITORY nu-book/zxing-cpp
        GIT_TAG v1.2.0
        )

    message("Fetch 'zxing-cpp' done.")
else()
    find_package(ZXing REQUIRED)
endif()
