file(STRINGS "${CMAKE_SOURCE_DIR}/misc/VERSION" GUI_VERSION)

configure_file(${CMAKE_SOURCE_DIR}/src/models/version.h.in
    ${CMAKE_BINARY_DIR}/generated/version_config.h
    )
