# TODO
# remove this included file from test/cpfhig/CMakeLists.txt
# link both statically AND shared object ways with the results of make install

cmake_minimum_required(VERSION 3.13.2)

project( cpfphig_system_tests
    VERSION 0.1
    LANGUAGES C
)

set( CPFPHIG_SOURCE_DIR ${CMAKE_SOURCE_DIR}/../../.. )

set( CMAKE_MODULE_PATH ${CPFPHIG_SOURCE_DIR}/cmake )

if( ${BUILD_TESTING} )
    enable_testing()
endif()

include( BuildTesting )
# TODO probably want to build it for system tests in a sub directory here
# or just make 2 directories, static_release, shared_release
function(cpfphig_add_build_commands BUILD_DIRECTORY )

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

set(${BUILD_DIRECTORY}_DEPEND ${BUILD_DIRECTORY}/src/cpfphig/cpfphig.cmake PARENT_SCOPE)

endfunction()

cpfphig_add_build_commands(
    shared_release_build
    -DBUILD_TESTING=NO
    -DBUILD_SHARED_LIBS=YES
    -DCMAKE_BUILD_TYPE=Release
    -DCMAKE_INSTALL_PREFIX=.
)
message( "${shared_release_build_DEPEND}" )
get_filename_component( shared_release_build_DEPEND_DIRECTORY ${shared_release_build_DEPEND} DIRECTORY )
# message( FATAL_ERROR "${CMAKE_CURRENT_BINARY_DIR}/${shared_release_build_DEPEND_DIRECTORY}" )

find_package(
    cpfphig
    CONFIG
    CONFIGS cpfphig.cmake
    PATHS ${CMAKE_CURRENT_BINARY_DIR}/${shared_release_build_DEPEND_DIRECTORY}
)

# cpfphig_add_build_commands(
#     static_release_build
#     -DBUILD_TESTING=NO
#     -DBUILD_SHARED_LIBS=NO
#     -DCMAKE_BUILD_TYPE=Release
#     -DCMAKE_INSTALL_PREFIX=.
# )


# System tests

find_package(cmocka REQUIRED)

function(cpfphig_add_system_test PREFIX )
    add_executable(${PREFIX}_system_test ${CMAKE_CURRENT_SOURCE_DIR}/${PREFIX}_system_test.c )

    target_include_directories( ${PREFIX}_system_test PRIVATE ${cmocka_INCLUDE_DIRS})
    target_link_libraries(${PREFIX}_system_test cpfphig ${cmocka_LIBRARIES} )

    add_test(system_${PREFIX} ${PREFIX}_system_test)
    list(APPEND COVERAGE_COMMANDS_LIST "COMMAND;${PREFIX}_system_test" )
    set(COVERAGE_COMMANDS_LIST ${COVERAGE_COMMANDS_LIST} PARENT_SCOPE)

endfunction()


# cpfphig_add_system_test(directory_list)
# 
# cpfphig_add_system_test(mutex)
# cpfphig_add_system_test(thread)
# cpfphig_add_system_test(thread_cond)
# cpfphig_add_system_test(thread_cond_broadcast)
# cpfphig_add_system_test(thread_pool)
 cpfphig_add_system_test(defer)
# 
# cpfphig_add_system_test(publisher_subscribe_publish_destroy)


# REMARK system tests that include "cpfphig.h" NOT "cpfphig/cpfphig.h"
# should be last

