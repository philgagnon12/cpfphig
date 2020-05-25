# cmocka
find_package(cmocka REQUIRED)


function(cpfphig_add_unit_test TEST_PREFIX )

    # Dont filter out when its has a CPFPHIG_REAL
    set( UNIT_TEST_SOURCES
        ${PROJECT_SOURCE_DIR}/src/${CMAKE_PROJECT_NAME}/${TEST_PREFIX}.c
        ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_PREFIX}_unit_test.c
    )
    foreach( ARG ${ARGN} )
        list( APPEND UNIT_TEST_SOURCES ${ARG} )
    endforeach()

    # Executable name
    string(REPLACE "/" "_" EXETARGET ${TEST_PREFIX})

    add_executable(${EXETARGET}_unit_test ${UNIT_TEST_SOURCES})

    if( WIN32 )
        foreach( RUNTIME_DLL "${cmocka_RUNTIME_DLLS}" )
            add_custom_command( TARGET ${EXETARGET}_unit_test
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${RUNTIME_DLL}" $<TARGET_FILE_DIR:${EXETARGET}_unit_test>
            )
        endforeach()
    endif()

    get_property( CPFPHIG_INCLUDE_DIRECTORIES TARGET ${CMAKE_PROJECT_NAME} PROPERTY INCLUDE_DIRECTORIES )
    target_include_directories( ${EXETARGET}_unit_test
        PRIVATE
        ${cmocka_INCLUDE_DIRS}
        ${CPFPHIG_INCLUDE_DIRECTORIES} # get_property up above ^
        ${CMAKE_CURRENT_SOURCE_DIR}/../..
    )
    target_link_libraries( ${EXETARGET}_unit_test ${cmocka_LIBRARIES})

    # CPFPHIG_REAL appending
    target_compile_definitions(${EXETARGET}_unit_test PRIVATE CPFPHIG_UNIT_TESTING)

    # coverage
    list(APPEND COVERAGE_COMMANDS_LIST "COMMAND;${EXETARGET}_unit_test" )
    set(COVERAGE_COMMANDS_LIST ${COVERAGE_COMMANDS_LIST} PARENT_SCOPE)

    # add test
    add_test(NAME test_${EXETARGET}_unit_test COMMAND ${EXETARGET}_unit_test)
endfunction()


cpfphig_add_unit_test(cpfphig
    mock/error_message_fprintf_mock.c
    mock/error_message_allocated_message_mock.c
)

cpfphig_add_unit_test(error_message_fprintf)

cpfphig_add_unit_test( error_message_allocated_message
    ${CMAKE_CURRENT_SOURCE_DIR}/mock/malloc_mock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mock/free_mock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mock/cpfphig_mock.c
)


cpfphig_add_unit_test(malloc
    ${CMAKE_CURRENT_SOURCE_DIR}/mock/cpfphig_mock.c
)

cpfphig_add_unit_test(free
    ${CMAKE_CURRENT_SOURCE_DIR}/mock/cpfphig_mock.c
)

cpfphig_add_unit_test(list_push
    ${CMAKE_CURRENT_SOURCE_DIR}/mock/cpfphig_mock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mock/malloc_mock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mock/free_mock.c
)

cpfphig_add_unit_test(list_shift
    ${CMAKE_CURRENT_SOURCE_DIR}/mock/cpfphig_mock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mock/list_push_mock.c
    ${PROJECT_SOURCE_DIR}/src/cpfphig/list_push.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mock/malloc_mock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mock/free_mock.c
)

cpfphig_add_unit_test(list_next
    ${CMAKE_CURRENT_SOURCE_DIR}/mock/cpfphig_mock.c
)


set(COVERAGE_COMMANDS_LIST ${COVERAGE_COMMANDS_LIST} PARENT_SCOPE)

