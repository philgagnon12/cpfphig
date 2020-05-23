cmake_minimum_required(VERSION 3.13.2)

project( cpfphig_system_tests
    VERSION 0.1
    LANGUAGES C
)

set( CMAKE_BUILD_TYPE RelWithDebInfo )

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
        COMMAND ${CMAKE_COMMAND} -G ${CMAKE_GENERATOR} ${CPFPHIG_SOURCE_DIR} ${BUILD_DEFINES}
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
    -DCMAKE_BUILD_TYPE=RelWithDebInfo
    -DCMAKE_INSTALL_PREFIX=.
)

cpfphig_install(
    static_release_build
    -DBUILD_TESTING=NO
    -DBUILD_SHARED_LIBS=NO
    -DCMAKE_BUILD_TYPE=RelWithDebInfo
    -DCMAKE_INSTALL_PREFIX=.
)


# System tests



function(cpfphig_add_system_test PREFIX )

    find_package(cmocka REQUIRED)

    foreach( ARG ${ARGN} )

        add_executable(${PREFIX}_${ARG}_system_test ${CMAKE_CURRENT_SOURCE_DIR}/${PREFIX}_system_test.c )

        foreach( RUNTIME_DLL "${cmocka_RUNTIME_DLLS}")
            add_custom_command( TARGET ${PREFIX}_${ARG}_system_test
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${RUNTIME_DLL}" $<TARGET_FILE_DIR:${PREFIX}_${ARG}_system_test>
            )
        endforeach()

        find_package(
            cpfphig
            CONFIG
            PATHS ${CMAKE_CURRENT_BINARY_DIR}/${${ARG}_CONFIG_PATH}
            REQUIRED
        )

        target_include_directories( ${PREFIX}_${ARG}_system_test PRIVATE ${cmocka_INCLUDE_DIRS})
        target_link_libraries(${PREFIX}_${ARG}_system_test cpfphig ${cmocka_LIBRARIES} )

        add_custom_command( TARGET ${PREFIX}_${ARG}_system_test
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:cpfphig>" $<TARGET_FILE_DIR:${PREFIX}_${ARG}_system_test>
        )


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

cpfphig_add_system_test(library ${BUILDS})
add_library( library MODULE library.c )
set_target_properties( library PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
target_compile_definitions( library_shared_release_build_system_test PUBLIC CPFPHIG_LIBRARY_PATH="$<TARGET_FILE:library>" )
target_compile_definitions( library_static_release_build_system_test PUBLIC CPFPHIG_LIBRARY_PATH="$<TARGET_FILE:library>" )

#REMARK system tests that include "cpfphig.h" NOT "cpfphig/cpfphig.h"
#should be last

