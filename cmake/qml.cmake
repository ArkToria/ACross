file(GLOB QML_FILES ${CMAKE_SOURCE_DIR}/src/views/*/*.qml)

list(APPEND QML_FILES
    ${CMAKE_SOURCE_DIR}/src/views/main.qml
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

foreach(icon ${ICON_FILES})
    string(REPLACE "${CMAKE_SOURCE_DIR}/" "" icon_alias ${icon})
    set_source_files_properties($icon PROPERTIES QT_RESOURCE_ALIAS ${icon_alias})
endforeach(icon)
