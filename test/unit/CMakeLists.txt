function(melphig_add_unit_test TEST_PREFIX )

    # Dont filter out when its has a REAL
    set( UNIT_TEST_SOURCES
         ${MELPHIG_SRC}/${TEST_PREFIX}.c
         ${MELPHIG_TEST}/unit/${TEST_PREFIX}_unit_test.c
         ${HEADER_LIST}
    )
    foreach( ARG ${ARGN} )
        list( APPEND UNIT_TEST_SOURCES ${ARG} )
    endforeach()

    # Executable name
    string(REPLACE "/" "_" EXETARGET ${TEST_PREFIX})
    add_executable(${EXETARGET}_unit_test ${UNIT_TEST_SOURCES})

    # cmocka
    find_package(cmocka REQUIRED)
    target_include_directories( ${EXETARGET}_unit_test PRIVATE ${cmocka_INCLUDE_DIRS})
    target_link_libraries( ${EXETARGET}_unit_test ${cmocka_LIBRARIES})

    # REAL appending
    target_compile_definitions(${EXETARGET}_unit_test PRIVATE MELPHIG_UNIT_TESTING)

    # coverage
    list(APPEND COVERAGE_COMMANDS_LIST "COMMAND;${EXETARGET}_unit_test" )
    set(COVERAGE_COMMANDS_LIST ${COVERAGE_COMMANDS_LIST} PARENT_SCOPE)

    # add test
    add_test(NAME test_${EXETARGET}_unit_test COMMAND ${EXETARGET}_unit_test)
endfunction()


melphig_add_unit_test(melphig)

melphig_add_unit_test(malloc
    ${CMAKE_CURRENT_SOURCE_DIR}/../mock/melphig_mock.c
)

melphig_add_unit_test(free
    ${CMAKE_CURRENT_SOURCE_DIR}/../mock/melphig_mock.c
)

melphig_add_unit_test(list_push
    ${CMAKE_CURRENT_SOURCE_DIR}/../mock/melphig_mock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../mock/malloc_mock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../mock/free_mock.c
)

melphig_add_unit_test(list_shift
    ${CMAKE_CURRENT_SOURCE_DIR}/../mock/melphig_mock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../mock/list_push_mock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/list_push.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../mock/malloc_mock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../mock/free_mock.c
)

melphig_add_unit_test(list_next
    ${CMAKE_CURRENT_SOURCE_DIR}/../mock/melphig_mock.c
)


set(COVERAGE_COMMANDS_LIST ${COVERAGE_COMMANDS_LIST} PARENT_SCOPE)

