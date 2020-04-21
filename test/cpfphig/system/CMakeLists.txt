find_package( Threads )

function(cpfphig_add_system_test PREFIX)
    add_executable(${PREFIX}_system_test ${CMAKE_CURRENT_SOURCE_DIR}/${PREFIX}_system_test.c )

    find_package(cmocka REQUIRED)
    target_include_directories( ${PREFIX}_system_test PRIVATE ${cmocka_INCLUDE_DIRS})
    target_link_libraries(${PREFIX}_system_test cpfphig ${cmocka_LIBRARIES} Threads::Threads m dl)

    add_test(system_${PREFIX} ${PREFIX}_system_test)
    list(APPEND COVERAGE_COMMANDS_LIST "COMMAND;${PREFIX}_system_test" )
    set(COVERAGE_COMMANDS_LIST ${COVERAGE_COMMANDS_LIST} PARENT_SCOPE)

endfunction()


cpfphig_add_system_test(directory_list)

cpfphig_add_system_test(mutex)
cpfphig_add_system_test(thread)
cpfphig_add_system_test(thread_cond)
cpfphig_add_system_test(thread_cond_broadcast)
cpfphig_add_system_test(thread_pool)
cpfphig_add_system_test(defer)

cpfphig_add_system_test(publisher_subscribe_publish_destroy)


# REMARK system tests that include "cpfphig.h" NOT "cpfphig/cpfphig.h"
# should be last

set(COVERAGE_COMMANDS_LIST ${COVERAGE_COMMANDS_LIST} PARENT_SCOPE)
