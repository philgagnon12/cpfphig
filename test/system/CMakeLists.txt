find_package( Threads )

function(melphig_add_system_test PREFIX)
    add_executable(${PREFIX}_system_test ${MELPHIG_TEST}/system/${PREFIX}_system_test.c )

    find_package(cmocka REQUIRED)
    target_include_directories( ${PREFIX}_system_test PRIVATE ${cmocka_INCLUDE_DIRS})
    target_link_libraries(${PREFIX}_system_test melphig ${cmocka_LIBRARIES} Threads::Threads m dl)

    add_test(system_${PREFIX} ${PREFIX}_system_test)
    list(APPEND COVERAGE_COMMANDS_LIST "COMMAND;${PREFIX}_system_test" )
    set(COVERAGE_COMMANDS_LIST ${COVERAGE_COMMANDS_LIST} PARENT_SCOPE)

endfunction()

add_subdirectory("plugins")

melphig_add_system_test(parse_notation)
melphig_add_system_test(parse_snprintf_note)
melphig_add_system_test(snprintf_note)
melphig_add_system_test(directory_list)

melphig_add_system_test(mutex)
melphig_add_system_test(thread)
melphig_add_system_test(thread_cond)
melphig_add_system_test(thread_cond_broadcast)
melphig_add_system_test(thread_pool)
melphig_add_system_test(defer)

melphig_add_system_test(publisher_subscribe_publish_destroy)


# REMARK system tests that include "melphig.h" NOT "melphig/melphig.h"
# should be last
melphig_add_system_test(plugin_load_all_symbols)

set(COVERAGE_COMMANDS_LIST ${COVERAGE_COMMANDS_LIST} PARENT_SCOPE)
