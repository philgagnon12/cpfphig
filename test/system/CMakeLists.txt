find_package( Threads )

function(fphig_add_system_test PREFIX)
    add_executable(${PREFIX}_system_test ${FPHIG_TEST}/system/${PREFIX}_system_test.c )

    find_package(cmocka REQUIRED)
    target_include_directories( ${PREFIX}_system_test PRIVATE ${cmocka_INCLUDE_DIRS})
    target_link_libraries(${PREFIX}_system_test fphig ${cmocka_LIBRARIES} Threads::Threads m dl)

    add_test(system_${PREFIX} ${PREFIX}_system_test)
    list(APPEND COVERAGE_COMMANDS_LIST "COMMAND;${PREFIX}_system_test" )
    set(COVERAGE_COMMANDS_LIST ${COVERAGE_COMMANDS_LIST} PARENT_SCOPE)

endfunction()

add_subdirectory("plugins")

fphig_add_system_test(parse_notation)
fphig_add_system_test(parse_snprintf_note)
fphig_add_system_test(snprintf_note)
fphig_add_system_test(directory_list)

fphig_add_system_test(mutex)
fphig_add_system_test(thread)
fphig_add_system_test(thread_cond)
fphig_add_system_test(thread_cond_broadcast)
fphig_add_system_test(thread_pool)
fphig_add_system_test(defer)

fphig_add_system_test(publisher_subscribe_publish_destroy)


# REMARK system tests that include "fphig.h" NOT "fphig/fphig.h"
# should be last
fphig_add_system_test(plugin_load_all_symbols)

set(COVERAGE_COMMANDS_LIST ${COVERAGE_COMMANDS_LIST} PARENT_SCOPE)
