if (UNIX)
    set(JSONCPP_INCLUDE_PATH "/usr/include/")
    set(JSONCPP_LINK_PATH "/usr/lib/")

    include_directories(${JSONCPP_INCLUDE_PATH})

    file(GLOB JSONCPP_HEADERS "${JSONCPP_INCLUDE_PATH}/json/*.h")

    find_library(JSONCPP_LIBRARY libjsoncpp.so REQUIRED)

    add_library(jsoncpp
        ${JSONCPP_HEADERS}
        )

    target_link_libraries(jsoncpp PUBLIC
        ${JSONCPP_LIBRARY}
        )
endif()
