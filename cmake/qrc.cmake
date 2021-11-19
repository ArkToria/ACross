file(GLOB QML_FILES ${CMAKE_SOURCE_DIR}/src/views/*/*.qml)

file(GLOB JS_FILES ${CMAKE_SOURCE_DIR}/src/views/*/*.js)

list(APPEND QML_FILES
    ${CMAKE_SOURCE_DIR}/src/views/main.qml
    ${JS_FILES}
    )

foreach(qml ${QML_FILES})
    string(REPLACE "${CMAKE_SOURCE_DIR}/" "" qml_alias ${qml})
    set_source_files_properties(${qml} PROPERTIES QT_RESOURCE_ALIAS ${qml_alias})
endforeach(qml)

file(GLOB ICON_FILES ${CMAKE_SOURCE_DIR}/misc/icons/*/*.svg)

list(APPEND ICON_FILES
    ${CMAKE_SOURCE_DIR}/misc/design/lines.svg
    ${CMAKE_SOURCE_DIR}/misc/design/logo.svg
    ${CMAKE_SOURCE_DIR}/misc/design/logo_pure.svg
    )
