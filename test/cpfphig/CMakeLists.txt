if(${BUILD_TESTING})
    include( BuildTesting )
    add_subdirectory(unit)

    # TODO add target for system tests
endif()