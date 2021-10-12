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
else()
    find_package(jsoncpp CONFIG REQUIRED)
    add_library(jsoncpp INTERFACE)
    target_link_libraries(jsoncpp INTERFACE jsoncpp_lib jsoncpp_object)
endif()

add_library(ACross::jsoncpp ALIAS jsoncpp)
