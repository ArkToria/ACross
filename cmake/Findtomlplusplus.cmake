if (ACROSS_TOMLPLUSPLUS_PROVIDER STREQUAL "module")
    set(TOMLPLUSPLUS_DIR ${CMAKE_SOURCE_DIR}/3rdpart/tomlplusplus)

    set(TOMLPLUSPLUS_INCLUDE_PATH
        ${TOMLPLUSPLUS_DIR}/include
        )

    set(TOMLPLUSPLUS_LIBRARY tomlplusplus)

    add_library(${TOMLPLUSPLUS_LIBRARY} STATIC
        ${TOMLPLUSPLUS_HEADERS}
        )

    target_include_directories(${TOMLPLUSPLUS_LIBRARY} PUBLIC
        ${TOMLPLUSPLUS_INCLUDE_PATH}
        )
endif ()

