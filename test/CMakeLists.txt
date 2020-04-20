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

    # Symlink for header files
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/../include/melphig_test
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/../include/melphig_test
        COMMENT "symlink include/melphig_test"
    )

    add_custom_target( symlink_include_melphig_test ALL
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/../include/melphig_test
    )
    # Initialize a list for code coverage

    add_subdirectory(assert)
    add_subdirectory(mock)
    add_subdirectory(unit)
    add_subdirectory(system)

    if(${COVERAGE})
        setup_target_for_coverage_lcov( NAME coverage
                                        EXECUTABLE ${COVERAGE_COMMANDS_LIST}
        )
    endif()

endif()
