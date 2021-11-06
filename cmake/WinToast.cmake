option(FETCH_WIN_TOAST "Download WinToast" ON)

if(FETCH_WIN_TOAST)
    CPMAddPackage(
        NAME WinToast
        GITHUB_REPOSITORY mohabouje/WinToast
        GIT_TAG 5e441fd03543b999edb663caf8df7be37c0d575c
        )
else()
    add_subdirectory(${CMAKE_SOURCE_DIR}/3rdpart/WinToast)
endif()

if (NOT TARGET WinToast::WinToast)
    add_library(WinToast::WinToast ALIAS WinToast)
endif()
