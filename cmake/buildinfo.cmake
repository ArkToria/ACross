file(STRINGS "${CMAKE_SOURCE_DIR}/misc/VERSION" GUI_VERSION)

set(BUILD_INFO "Manually Build" CACHE STRING "Display BuildInfo")

set(FONTS_FALLBACK "Noto Sans, Segoe UI Variable, Segoe UI, Noto Sans CJK, PingFang SC, Microsoft YaHei UI" CACHE STRING "Fonts Fallback")

set(DATE_TIME_FORMAT "MM/dd/yyyy hh:mm:ss" CACHE STRING "Default date time format")

configure_file(${CMAKE_SOURCE_DIR}/src/view_models/buildinfo.h.in
    ${CMAKE_BINARY_DIR}/generated/buildinfo.h
    )
