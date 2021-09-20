set(TRANS_DIR "${CMAKE_SOURCE_DIR}/i18n")

set(TS_FILES
    ${TRANS_DIR}/across_zh_CN.ts
    )

find_package(Qt6 COMPONENTS LinguistTools REQUIRED)

qt_create_translation(ACROSS_QM_FILES "${CMAKE_SOURCE_DIR}/src" ${TS_FILES})

configure_file(${TRANS_DIR}/i18n.qrc ${CMAKE_BINARY_DIR} COPYONLY)

set(ACROSS_I18N_QRC ${CMAKE_BINARY_DIR}/i18n.qrc)
