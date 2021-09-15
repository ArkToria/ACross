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
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake;")
set(ACROSS_TOMLPLUSPLUS_PROVIDER "module")

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick Widgets DBus REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Widgets DBus REQUIRED)

find_package(CURL REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(jsoncpp REQUIRED)
find_package(magic_enum REQUIRED)
find_package(tomlplusplus REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(spdlog REQUIRED)
find_package(Sodium REQUIRED)
find_package(ZXing REQUIRED)
find_package(Threads REQUIRED)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/protobuf.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/grpc.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/translations.cmake")
##################################

######## SOURCES FILE ########
include_directories(${CMAKE_BINARY_DIR}/generated/)

set(QRC_FILES
    qml.qrc
    ${ACROSS_I18N_QRC}
    )

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
    src/models/apitools.h
    src/models/cryptotools.h
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
    ${API_PROTO_HEADER}
    ${API_GRPC_HEADER}
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
    src/models/apitools.cpp
    src/models/cryptotools.cpp
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
    ${QRC_FILES}
    ${ACROSS_QM_FILES}
    ${API_PROTO_SOURCE}
    ${API_GRPC_SOURCE}
    )
##############################

######## COMPILE ########

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(across
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
        )
else ()
    add_executable(across
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
        )
endif ()

target_compile_definitions(across
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(across PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::DBus
    CURL::libcurl
    fmt::fmt
    gRPC::grpc++
    gRPC::grpc++_reflection
    jsoncpp
    magic_enum
    pthread
    sodium
    spdlog::spdlog
    SQLite::SQLite3
    tomlplusplus
    ZXing
    )

if (QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(across)
    qt_finalize_executable(across)
endif ()

######## UNIT_TEST ########
enable_testing()
find_package(GTest REQUIRED)

add_subdirectory(tests)
###########################

#########################
######## INSTALL ########
#########################
install(TARGETS across RUNTIME DESTINATION bin)
install(FILES ${CMAKE_SOURCE_DIR}/misc/org.arktoria.across.desktop DESTINATION share/applications)
install(FILES ${CMAKE_SOURCE_DIR}/misc/across_example.toml DESTINATION share/across/)
install(FILES ${CMAKE_SOURCE_DIR}/misc/design/logo.svg DESTINATION share/icons/hicolor/scalable/apps/ RENAME org.arktoria.across.svg)
install(FILES ${CMAKE_SOURCE_DIR}/misc/icons/light/tray_logo_running.svg DESTINATION share/icons/hicolor/scalable/apps/ RENAME org.arktoria.across.light.running.svg)
install(FILES ${CMAKE_SOURCE_DIR}/misc/icons/light/tray_logo_stop.svg DESTINATION share/icons/hicolor/scalable/apps/ RENAME org.arktoria.across.light.stop.svg)
install(FILES ${CMAKE_SOURCE_DIR}/misc/icons/dark/tray_logo_running.svg DESTINATION share/icons/hicolor/scalable/apps/ RENAME org.arktoria.across.dark.running.svg)
install(FILES ${CMAKE_SOURCE_DIR}/misc/icons/dark/tray_logo_stop.svg DESTINATION share/icons/hicolor/scalable/apps/ RENAME org.arktoria.across.dark.stop.svg)
