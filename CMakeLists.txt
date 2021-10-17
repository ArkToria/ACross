######## CMAKE CONFIG ########
cmake_minimum_required(VERSION 3.20)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
##############################

######### PRE_BUILD #########
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/buildinfo.cmake")
#############################

project(across VERSION ${GUI_VERSION} LANGUAGES CXX)

######## FIND DEPENDENCES ########
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/CPM.cmake")
set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/3rdpart")
set(CPM_USE_LOCAL_PACKAGES ON)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/protobuf.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/grpc.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Semver.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/MagicEnum.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/SingleApplication.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/qrc.cmake")

# Qt 6.2
find_package(Qt6 6.2 COMPONENTS Core Quick Widgets DBus Core5Compat Sql LinguistTools Concurrent REQUIRED)

# curl
CPMAddPackage(
    NAME CURL
    GITHUB_REPOSITORY "curl/curl"
    VERSION 7.74.0
    )

# fmt
CPMAddPackage(
    NAME fmt
    GITHUB_REPOSITORY "fmtlib/fmt"
    VERSION 8.0.0
    )

# spdlog
CPMAddPackage(
    NAME spdlog
    GITHUB_REPOSITORY "gabime/spdlog"
    VERSION 1.9.2
    OPTIONS "SPDLOG_BUILD_BENCH OFF" "SPDLOG_FMT_EXTERNAL ON" "SPDLOG_BUILD_SHARED ON" "SPDLOG_BUILD_TESTS OFF"
    )

# ZXing
CPMAddPackage(
    NAME ZXing
    GITHUB_REPOSITORY "nu-book/zxing-cpp"
    VERSION 1.1.1
    OPTIONS "BUILD_EXAMPLES OFF" "BUILD_BLACKBOX_TESTS OFF"
    )

# JSON
CPMAddPackage(
    NAME nlohmann_json
    GITHUB_REPOSITORY "nlohmann/json"
    VERSION 3.9.1
    OPTIONS "JSON_BuildTests OFF"
    )

find_package(Threads REQUIRED)

# BEGIN Special case for QtCreator
list(APPEND QML_DIRS "${CMAKE_CURRENT_BINARY_DIR}")
set(QML_IMPORT_PATH "${QML_DIRS}" CACHE STRING "Qt Creator extra qml import paths")
# END Special case

##################################

######## SOURCE FILES ########
include_directories(${CMAKE_BINARY_DIR}/generated/)

set(PROJECT_HEADERS
    src/app.h
    src/view_models/logview.h
    src/view_models/logtools.h
    src/view_models/configtools.h
    src/view_models/grouplist.h
    src/view_models/groupmodel.h
    src/view_models/nodelist.h
    src/view_models/nodemodel.h
    src/view_models/nodeformmodel.h
    src/view_models/trojanformmodel.h
    src/view_models/shadowsocksformmodel.h
    src/view_models/vmessformmodel.h
    src/view_models/rawoutboundformmodel.h
    src/view_models/urlschemeformmodel.h
    src/view_models/buildinfo.h.in
    src/view_models/systemtray.h
    src/view_models/imageprovider.h
    src/models/apitools.h
    src/models/dbtools.h
    src/models/envtools.h
    src/models/qrcodetools.h
    src/models/networktools.h
    src/models/jsontools.h
    src/models/serializetools.h
    src/models/clipboardtools.h
    src/models/notifytools.h
    src/models/dbustools.h
    src/models/coretools.h
    src/models/confighelper.h
    ${API_PROTO_HEADER}
    ${API_GRPC_HEADER}
    ${ACROSS_PROTO_HEADER}
    ${ACROSS_GRPC_HEADER}
    )

set(PROJECT_SOURCES
    src/main.cpp
    src/app.cpp
    src/view_models/logview.cpp
    src/view_models/logtools.cpp
    src/view_models/configtools.cpp
    src/view_models/grouplist.cpp
    src/view_models/groupmodel.cpp
    src/view_models/nodelist.cpp
    src/view_models/nodemodel.cpp
    src/view_models/nodeformmodel.cpp
    src/view_models/trojanformmodel.cpp
    src/view_models/shadowsocksformmodel.cpp
    src/view_models/vmessformmodel.cpp
    src/view_models/rawoutboundformmodel.cpp
    src/view_models/urlschemeformmodel.cpp
    src/view_models/systemtray.cpp
    src/view_models/imageprovider.cpp
    src/models/apitools.cpp
    src/models/dbtools.cpp
    src/models/envtools.cpp
    src/models/qrcodetools.cpp
    src/models/networktools.cpp
    src/models/jsontools.cpp
    src/models/serializetools.cpp
    src/models/clipboardtools.cpp
    src/models/notifytools.cpp
    src/models/dbustools.cpp
    src/models/coretools.cpp
    src/models/confighelper.cpp
    ${ACROSS_QM_FILES}
    ${ACROSS_PROTO_SOURCE}
    ${ACROSS_GRPC_SOURCE}
    ${API_PROTO_SOURCE}
    ${API_GRPC_SOURCE}
    )
##############################

######## COMPILE ########
qt_add_executable(across
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    )

target_include_directories(across PRIVATE $<TARGET_PROPERTY:ZXing::Core,INTERFACE_INCLUDE_DIRECTORIES>/ZXing)
target_compile_definitions(across
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(across PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::Widgets
    Qt6::DBus
    Qt6::Core5Compat
    Qt6::Sql
    Qt6::Concurrent
    CURL::libcurl
    fmt::fmt
    gRPC::grpc++
    gRPC::grpc++_reflection
    magic_enum::magic_enum
    nlohmann_json::nlohmann_json
    semver::semver
    SingleApplication::SingleApplication
    spdlog::spdlog
    Threads::Threads
    ZXing::Core
    )

qt_add_qml_module(across
    URI "ACross"
    VERSION 1.0
    RESOURCE_PREFIX "/"
    QML_FILES
    ${QML_FILES}
    )

qt_add_translations(across
    RESOURCE_PREFIX "/i18n"
    TS_FILES
    ${CMAKE_SOURCE_DIR}/i18n/across_zh_CN.ts
    )

qt_add_resources(across misc
    PREFIX "/misc/"
    BASE "${CMAKE_CURRENT_LIST_DIR}/misc"
    FILES
    ${ICON_FILES}
    ${CMAKE_SOURCE_DIR}/misc/across_example.json
    ${CMAKE_SOURCE_DIR}/misc/across.proto
    )

qt_import_qml_plugins(across)
qt_finalize_executable(across)
add_dependencies(across across_lupdate)

set_target_properties(across PROPERTIES
    WIN32_EXECUTABLE TRUE)

if (WIN32)
    add_custom_command(
        TARGET across
        POST_BUILD
        COMMAND "${Qt6_DIR}/../../../bin/windeployqt" "${CMAKE_BINARY_DIR}/across.exe" "--qmldir" "${CMAKE_SOURCE_DIR}/src/views"
        COMMENT "Deploying Qt..."
        )
endif()

######## UNIT_TEST ########

###########################

#########################
######## INSTALL ########
#########################
install(TARGETS across RUNTIME DESTINATION bin)
install(FILES ${CMAKE_SOURCE_DIR}/misc/org.arktoria.across.desktop DESTINATION share/applications)
install(FILES ${CMAKE_SOURCE_DIR}/misc/across_example.json DESTINATION share/across/)
install(FILES ${CMAKE_SOURCE_DIR}/misc/design/logo.svg DESTINATION share/icons/hicolor/scalable/apps/ RENAME org.arktoria.across.svg)
install(FILES ${CMAKE_SOURCE_DIR}/misc/icons/light/tray_logo_running.svg DESTINATION share/icons/hicolor/scalable/apps/ RENAME org.arktoria.across.light.running.svg)
install(FILES ${CMAKE_SOURCE_DIR}/misc/icons/light/tray_logo_stop.svg DESTINATION share/icons/hicolor/scalable/apps/ RENAME org.arktoria.across.light.stop.svg)
install(FILES ${CMAKE_SOURCE_DIR}/misc/icons/dark/tray_logo_running.svg DESTINATION share/icons/hicolor/scalable/apps/ RENAME org.arktoria.across.dark.running.svg)
install(FILES ${CMAKE_SOURCE_DIR}/misc/icons/dark/tray_logo_stop.svg DESTINATION share/icons/hicolor/scalable/apps/ RENAME org.arktoria.across.dark.stop.svg)
