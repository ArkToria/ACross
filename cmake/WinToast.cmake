option(FETCH_WIN_TOAST "Download WinToast" ON)

if(FETCH_WIN_TOAST)
    CPMAddPackage(
        NAME win_toast
        GITHUB_REPOSITORY mohabouje/WinToast
        GIT_TAG 5e441fd03543b999edb663caf8df7be37c0d575c
        )
else()
    set(WIN_TOAST_DIR ${CMAKE_SOURCE_DIR}/3rdpart/WinToast)

    set(WIN_TOAST_INCLUDE_PATH ${WIN_TOAST_DIR}/src)

    set(WIN_TOAST_LIBRARY win_toast)

    add_library(${WIN_TOAST_LIBRARY} STATIC
        ${WIN_TOAST_INCLUDE_PATH}/wintoastlib.cpp
        )

    target_include_directories(${WIN_TOAST_LIBRARY} INTERFACE
        ${WIN_TOAST_INCLUDE_PATH}
        )
endif()

if (NOT TARGET win_toast::win_toast)
    add_library(win_toast::win_toast ALIAS win_toast)
endif()
