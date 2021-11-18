######## CMAKE CONFIG ########
cmake_minimum_required(VERSION 3.18)

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

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/grpc.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Semver.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/MagicEnum.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/SingleApplication.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/qrc.cmake")

# Qt 6.2
find_package(Qt6 6.2 COMPONENTS Gui Core Quick Widgets DBus 
    Core5Compat Sql LinguistTools Concurrent ToolsTools REQUIRED)

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
    VERSION 7.1.3
    )

# spdlog
CPMAddPackage(
    NAME spdlog
    GITHUB_REPOSITORY "gabime/spdlog"
    VERSION 1.9.2
    OPTIONS "SPDLOG_BUILD_BENCH OFF" "SPDLOG_FMT_EXTERNAL ON" "SPDLOG_BUILD_SHARED ON" "SPDLOG_BUILD_TESTS OFF"
    )

# A hack to prevent installing ZXing stuffs.
install(CODE [[set(CMAKE_INSTALL_LOCAL_ONLY ON)]])

# ZXing
CPMAddPackage(
    NAME ZXing
    GITHUB_REPOSITORY "nu-book/zxing-cpp"
    VERSION 1.2.0
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
include_directories(${CMAKE_CURRENT_BINARY_DIR}/generated/)

set(PROJECT_HEADERS
    src/app.h
    src/view_models/logview.h
    src/view_models/configtools.h
    src/view_models/grouplist.h
    src/view_models/groupmodel.h
    src/view_models/nodelist.h
    src/view_models/nodemodel.h
    src/view_models/nodeformmodel.h
    src/view_models/buildinfo.h.in
    src/view_models/systemtray.h
    src/view_models/imageprovider.h
    src/view_models/jsonhighlighter.h
    src/view_models/loghighlighter.h
    src/models/apitools.h
    src/models/dbtools.h
    src/models/envtools.h
    src/models/qrcodetools.h
    src/models/networktools.h
    src/models/serializetools.h
    src/models/clipboardtools.h
    src/models/notifytools.h
    src/models/dbustools.h
    src/models/coretools.h
    src/models/confighelper.h
    ${GRPC_HEADERS}
    ${PROTO_HEADERS}
    )

set(PROJECT_SOURCES
    src/main.cpp
    src/app.cpp
    src/view_models/logview.cpp
    src/view_models/configtools.cpp
    src/view_models/grouplist.cpp
    src/view_models/groupmodel.cpp
    src/view_models/nodelist.cpp
    src/view_models/nodemodel.cpp
    src/view_models/nodeformmodel.cpp
    src/view_models/systemtray.cpp
    src/view_models/imageprovider.cpp
    src/view_models/jsonhighlighter.cpp
    src/view_models/loghighlighter.cpp
    src/models/apitools.cpp
    src/models/dbtools.cpp
    src/models/envtools.cpp
    src/models/qrcodetools.cpp
    src/models/networktools.cpp
    src/models/serializetools.cpp
    src/models/clipboardtools.cpp
    src/models/notifytools.cpp
    src/models/dbustools.cpp
    src/models/coretools.cpp
    src/models/confighelper.cpp
    ${ACROSS_QM_FILES}
    ${GRPC_SOURCES}
    ${PROTO_SOURCES}
    )

##############################

######## COMPILE ########
qt_add_executable(across
    MANUAL_FINALIZATION
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
    misc/across.rc
    )

if(NOT ZXing_ADDED)
    # Include header files from system package or fetch content with the sample path
    target_include_directories(across PRIVATE $<TARGET_PROPERTY:ZXing::Core,INTERFACE_INCLUDE_DIRECTORIES>/ZXing)
endif()

target_compile_definitions(across
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(across PRIVATE
    Qt::Core
    Qt::Quick
    Qt::Widgets
    Qt::DBus
    Qt::Core5Compat
    Qt::Sql
    Qt::Concurrent
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

if(WIN32)
    set_target_properties(across PROPERTIES WIN32_EXECUTABLE TRUE)

    if (MSVC)
        # For CppWinRT pregenerated headers in Windows SDK
        target_compile_options(across PRIVATE "/await" "/Zc:twoPhase-")

        target_link_libraries(across PRIVATE WindowsApp.lib)
    endif()

    target_link_libraries(across PRIVATE
        wsock32
        ws2_32
        )
endif()

qt_add_qml_module(across
    URI "ACross"
    VERSION 1.0
    RESOURCE_PREFIX "/"
    QML_FILES
    ${QML_FILES}
    )

qt_add_translations(across
    TS_FILES ${CMAKE_SOURCE_DIR}/i18n/across_zh_CN.ts
    RESOURCE_PREFIX "/i18n/"
    )

qt_add_resources(across misc
    PREFIX "/misc/"
    BASE "${CMAKE_CURRENT_LIST_DIR}/misc"
    FILES
    ${ICON_FILES}
    ${CMAKE_SOURCE_DIR}/misc/across_example.json
    ${CMAKE_SOURCE_DIR}/misc/across.proto
    ${CMAKE_SOURCE_DIR}/misc/v2ray_config.proto
    )

qt_import_qml_plugins(across)

qt_finalize_executable(across)

# update translation
# add_dependencies(across across_lupdate)

######## UNIT_TEST ########

###########################

#########################
######## INSTALL ########
#########################
option(ENABLE_DEPLOYMENT "Enable fixup_bundle and windeployqt when installing." ON)

if(WIN32 AND ENABLE_DEPLOYMENT)
    install(TARGETS across RUNTIME DESTINATION .)

    # For Global Target
    list(APPEND DIRS "${CMAKE_BINARY_DIR}")
    list(APPEND DIRS "${CMAKE_CURRENT_BINARY_DIR}")
    list(APPEND DIRS "${Qt6Core_DIR}/../../")
    list(APPEND DIRS "${Qt6Core_DIR}/../../../bin/")

    if(CMAKE_PREFIX_PATH)
        foreach(dir ${CMAKE_PREFIX_PATH})
            list(APPEND DIRS "${dir}/bin" "${dir}/lib")
        endforeach()
    endif()

    list(APPEND DIRS "${Qt6Core_DIR}/../..")

    foreach(d ${DIRS})
        string(REPLACE "\\" "/" newd ${d})
        list(APPEND NEW_DIRS ${newd})
    endforeach()
    set(DIRS ${NEW_DIRS})

    set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION .)

    set(APP "${CMAKE_INSTALL_PREFIX}/across.exe")
    install(CODE "include(BundleUtilities)")
    install(CODE "fixup_bundle(\"${APP}\"   \"\"   \"${DIRS}\")")
    install(CODE "message(\"Now running windeployqt\")")
    install(CODE "execute_process(COMMAND \"$<TARGET_FILE:Qt::windeployqt>\" \"${APP}\" --no-virtualkeyboard --no-compiler-runtime --no-opengl-sw -qmldir=${CMAKE_CURRENT_SOURCE_DIR}/src/views/ -qmlimport=.)")
    install(CODE "message(\"Completed\")")
else()
    install(TARGETS across RUNTIME DESTINATION bin)
    install(FILES ${CMAKE_SOURCE_DIR}/misc/org.arktoria.across.desktop DESTINATION share/applications)
    install(FILES ${CMAKE_SOURCE_DIR}/misc/design/logo.svg DESTINATION share/icons/hicolor/scalable/apps/ RENAME org.arktoria.across.svg)
    install(FILES ${CMAKE_SOURCE_DIR}/misc/icons/light/tray_logo_running.svg DESTINATION share/icons/hicolor/scalable/apps/ RENAME org.arktoria.across.light.running.svg)
    install(FILES ${CMAKE_SOURCE_DIR}/misc/icons/light/tray_logo_stop.svg DESTINATION share/icons/hicolor/scalable/apps/ RENAME org.arktoria.across.light.stop.svg)
    install(FILES ${CMAKE_SOURCE_DIR}/misc/icons/dark/tray_logo_running.svg DESTINATION share/icons/hicolor/scalable/apps/ RENAME org.arktoria.across.dark.running.svg)
    install(FILES ${CMAKE_SOURCE_DIR}/misc/icons/dark/tray_logo_stop.svg DESTINATION share/icons/hicolor/scalable/apps/ RENAME org.arktoria.across.dark.stop.svg)
endif()

######## DEBUG ALL VARIABLES ########
#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
#################################
