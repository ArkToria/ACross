option(FETCH_WIN_TOAST "Download WinToast" ON)

if(FETCH_WIN_TOAST)
    CPMAddPackage(
        NAME WinToast
        GITHUB_REPOSITORY moodyhunter/WinToast
        GIT_TAG cf03283a030d0d043120f4cfed813828cb907899
        )
else()
    add_subdirectory(${CMAKE_SOURCE_DIR}/3rdpart/WinToast)
endif()

if (NOT TARGET WinToast::WinToast)
    add_library(WinToast::WinToast ALIAS WinToast)
endif()
