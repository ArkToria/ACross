set(TRANS_DIR "${CMAKE_SOURCE_DIR}/i18n")

set(TS_FILES
    ${TRANS_DIR}/across_zh_CN.ts
    )

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Linguist REQUIRED)

     qt_create_translation(ACROSS_QM_FILES "${CMAKE_SOURCE_DIR}/src" ${TS_FILES})
else ()
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS LinguistTools REQUIRED)

     qt5_create_translation(ACROSS_QM_FILES "${CMAKE_SOURCE_DIR}/src" ${TS_FILES})
endif ()

configure_file(${TRANS_DIR}/i18n.qrc ${CMAKE_BINARY_DIR} COPYONLY)

set(ACROSS_I18N_QRC ${CMAKE_BINARY_DIR}/i18n.qrc)
