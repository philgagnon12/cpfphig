cmake_minimum_required(VERSION 3.13.2)

project( cpfphig_system_tests
    VERSION 0.1
    LANGUAGES C
)

set( CPFPHIG_SOURCE_DIR ${CMAKE_SOURCE_DIR}/../../.. )

set( CMAKE_MODULE_PATH ${CPFPHIG_SOURCE_DIR}/cmake )

enable_testing()

function(cpfphig_install BUILD_DIRECTORY )

    set(BUILD_DEFINES)
    foreach( ARG ${ARGN} )
        list( APPEND BUILD_DEFINES "${ARG} " )
    endforeach()

    execute_process(
        COMMAND ${CMAKE_COMMAND} -E make_directory ${BUILD_DIRECTORY}
    )

    execute_process(
        COMMAND ${CMAKE_COMMAND} ${CPFPHIG_SOURCE_DIR} ${BUILD_DEFINES}
        WORKING_DIRECTORY ${BUILD_DIRECTORY}
    )

    execute_process(
        COMMAND ${CMAKE_COMMAND} --build .
        WORKING_DIRECTORY ${BUILD_DIRECTORY}
    )

    execute_process(
        COMMAND ${CMAKE_COMMAND} --build . --target install
        WORKING_DIRECTORY ${BUILD_DIRECTORY}
    )

set(${BUILD_DIRECTORY}_CONFIG_PATH ${BUILD_DIRECTORY}/src/cpfphig PARENT_SCOPE)

endfunction()

cpfphig_install(
    shared_release_build
    -DBUILD_TESTING=NO
    -DBUILD_SHARED_LIBS=YES
    -DCMAKE_BUILD_TYPE=Release
    -DCMAKE_INSTALL_PREFIX=.
)

cpfphig_install(
    static_release_build
    -DBUILD_TESTING=NO
    -DBUILD_SHARED_LIBS=NO
    -DCMAKE_BUILD_TYPE=Release
    -DCMAKE_INSTALL_PREFIX=.
)


# System tests

find_package(cmocka REQUIRED)

function(cpfphig_add_system_test PREFIX )

    foreach( ARG ${ARGN} )

        add_executable(${PREFIX}_${ARG}_system_test ${CMAKE_CURRENT_SOURCE_DIR}/${PREFIX}_system_test.c )

        find_package(
            cpfphig
            CONFIG
            PATHS ${CMAKE_CURRENT_BINARY_DIR}/${${ARG}_CONFIG_PATH}
            REQUIRED
        )

        target_include_directories( ${PREFIX}_${ARG}_system_test PRIVATE ${cmocka_INCLUDE_DIRS})
        target_link_libraries(${PREFIX}_${ARG}_system_test cpfphig ${cmocka_LIBRARIES} )

        add_test(system_${PREFIX}_${ARG} ${PREFIX}_${ARG}_system_test)
        list(APPEND COVERAGE_COMMANDS_LIST "COMMAND;${PREFIX}_${ARG}_system_test" )
        set(COVERAGE_COMMANDS_LIST ${COVERAGE_COMMANDS_LIST} PARENT_SCOPE)


    endforeach()


endfunction()

set(BUILDS
    shared_release_build
    static_release_build
)

cpfphig_add_system_test(directory_list ${BUILDS})

cpfphig_add_system_test(mutex ${BUILDS})
cpfphig_add_system_test(thread ${BUILDS})
cpfphig_add_system_test(thread_cond ${BUILDS})
cpfphig_add_system_test(thread_cond_broadcast ${BUILDS})
cpfphig_add_system_test(thread_pool ${BUILDS})
cpfphig_add_system_test(defer ${BUILDS})

cpfphig_add_system_test(publisher_subscribe_publish_destroy ${BUILDS})


#REMARK system tests that include "cpfphig.h" NOT "cpfphig/cpfphig.h"
#should be last

