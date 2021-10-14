option(FETCH_SPDLOG "Download spdlog" OFF)

if(FETCH_SPDLOG)
    CPMAddPackage(
        NAME spdlog
        GITHUB_REPOSITORY gabime/spdlog
        GIT_TAG eb3220622e73a4889eee355ffa37972b3cac3df5 # v1.9.2
        )
else()
    find_package(spdlog REQUIRED NO_DEFAULT_PATH)
endif()
