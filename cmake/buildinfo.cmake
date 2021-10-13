file(STRINGS "${CMAKE_SOURCE_DIR}/misc/VERSION" GUI_VERSION)

set(BUILD_INFO "Manually Build" CACHE STRING "Display BuildInfo")

set(FONTS_FALLBACK "Noto Sans, Segoe UI Variable, Segoe UI, Noto Sans CJK, PingFang SC, Microsoft YaHei UI" CACHE STRING "Fonts Fallback")

configure_file(${CMAKE_SOURCE_DIR}/src/view_models/buildinfo.h.in
    ${CMAKE_BINARY_DIR}/generated/buildinfo.h
    )
