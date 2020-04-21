if(${BUILD_TESTING})

    if(${COVERAGE})
        include(CodeCoverage)
        append_coverage_compiler_flags()
        # Remove all the crap targets from including CTest
        set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
        # CTest needed for valgrind
        # TODO add custom target for valgrind, if valgrind is installed
        include(CTest)
    endif()

    add_subdirectory(unit)
    add_subdirectory(system)

    if(${COVERAGE})
        setup_target_for_coverage_lcov( NAME coverage
                                        EXECUTABLE ${COVERAGE_COMMANDS_LIST}
        )
    endif()

endif()
