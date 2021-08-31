file(STRINGS "${CMAKE_SOURCE_DIR}/misc/VERSION" GUI_VERSION)

set(BUILD_INFO "Manually Build")

configure_file(${CMAKE_SOURCE_DIR}/src/view_models/buildinfo.h.in
    ${CMAKE_BINARY_DIR}/generated/buildinfo.h
    )
