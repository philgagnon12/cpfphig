# cmake .. -DBUILD_TESTING=YES -DCMAKE_BUILD_TYPE=Debug -DCMAKE_COMPILER_IS_GNUCXX=YES -DCOVERAGE=YES 

# -DBUILD_SHARED_LIBS=YES
# -CMAKE_INSTALL_PREFIX=.

# TODO add warning flags to compiler

cmake_minimum_required(VERSION 3.13.2)

project( cpfphig
    VERSION 0.1
    LANGUAGES C
)


include(CheckIncludeFiles)

check_include_files("dlfcn.h"   CPFPHIG_HAVE_DLFCN_H)
check_include_files("windows.h" CPFPHIG_HAVE_WINDOWS_H)
check_include_files("dirent.h"  CPFPHIG_HAVE_DIRENT_H)
check_include_files("libgen.h"  CPFPHIG_HAVE_LIBGEN_H)
check_include_files("pthread.h" CPFPHIG_HAVE_PTHREAD_H)
check_include_files("unistd.h"  CPFPHIG_HAVE_UNISTD_H)

include(CheckFunctionExists)

check_function_exists(strnstr CPFPHIG_HAVE_STRNSTR)

configure_file( cpfphig_config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/src/cpfphig/cpfphig_config.h )

set( CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )


if( ${BUILD_TESTING} )
    enable_testing()
endif()


# TODO RPATH HANDLING
# https://gitlab.kitware.com/cmake/community/-/wikis/doc/cmake/RPATH-handling
# Currently the below setting will use absolute path for RPATH, when doing an install package this needs to be dynamic
# SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

add_subdirectory("src/cpfphig")

add_subdirectory("test/cpfphig")

# ctags
find_program( CPFPHIG_CTAGS NAMES ctags )
if( CPFPHIG_CTAGS )
    execute_process( COMMAND ${CPFPHIG_CTAGS} --version
        RESULT_VARIABLE CTAGS_RETURN
        OUTPUT_QUIET
        ERROR_QUIET
    )
if( CTAGS_RETURN EQUAL 0 )
        add_custom_target( ctags ALL )
        add_custom_command( TARGET ctags
            POST_BUILD
            COMMAND ${CPFPHIG_CTAGS} -R --exclude=.git src/${CMAKE_PROJECT_NAME} test/${CMAKE_PROJECT_NAME}/unit/mock
            WORKING_DIRECTORY ${cpfphig_SOURCE_DIR}
            COMMENT "running ctags"
        )
    else()
        message( WARNING "ctags is not Exuberant ctags" )
    endif( CTAGS_RETURN EQUAL 0 )
else()
    message( WARNING "ctags not found" )
endif( CPFPHIG_CTAGS )
