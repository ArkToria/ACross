file(GLOB QML_FILES ${CMAKE_SOURCE_DIR}/src/views/*/*.qml)

foreach(qml ${QML_FILES})
    string(REPLACE "${CMAKE_CURRENT_LIST_DIR}/" "" qml_alias ${qml})
    set_source_files_properties(${qml} PROPERTIES QT_RESOURCE_ALIAS ${qml_alias})
endforeach(qml)
