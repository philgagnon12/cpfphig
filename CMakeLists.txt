# cmake .. -DBUILD_TESTING=YES -DCMAKE_BUILD_TYPE=Debug -DCMAKE_COMPILER_IS_GNUCXX=YES -DCOVERAGE=YES 

# TODO add warning flags to compiler

cmake_minimum_required(VERSION 3.13.2)

project( fphig
         VERSION 0.1
         LANGUAGES C )

option(FPHIG_WITH_FLUIDSYNTH  "Input_Driver and Output_Driver" OFF)
option(FPHIG_WITH_LUA         "Lua plugin" OFF)

# Default values
set(FPHIG_CONST_MPHIG_DEFAULT_BPM             "120"     CACHE STRING "default bpm when initialized")
set(FPHIG_CONST_MPHIG_DEFAULT_SILENCE_VALUE   "1.0"     CACHE STRING "default silence value when initialized")

set(FPHIG_CONST_MPHIG_PARSED_NOTE_DEFAULT_KEY         "C"     CACHE STRING "struct fphig_parsed_note default value for member key")
set(FPHIG_CONST_MPHIG_PARSED_NOTE_DEFAULT_OCTAVE      "5"     CACHE STRING "struct fphig_parsed_note default value for member octave")
set(FPHIG_CONST_MPHIG_PARSED_NOTE_DEFAULT_VALUE       "1.0"   CACHE STRING "struct fphig_parsed_note default value for member value")
set(FPHIG_CONST_MPHIG_PARSED_NOTE_DEFAULT_SIGNATURE   "0"     CACHE STRING "struct fphig_parsed_note default value for member signature")
set(FPHIG_CONST_MPHIG_PARSED_NOTE_DEFAULT_VELOCITY    "60"    CACHE STRING "struct fphig_parsed_note default value for member velocity")

set(FPHIG_CONST_MPHIG_NOTE_DEFAULT_KEY         "60"    CACHE STRING "struct fphig_note default value for member key")
set(FPHIG_CONST_MPHIG_NOTE_DEFAULT_VALUE       "1.0"   CACHE STRING "struct fphig_note default value for member value")
set(FPHIG_CONST_MPHIG_NOTE_DEFAULT_VELOCITY    "60"    CACHE STRING "struct fphig_note default value for member velocity")


include(CheckIncludeFiles)

check_include_files("dlfcn.h"   FPHIG_HAVE_DLFCN_H)
check_include_files("windows.h" FPHIG_HAVE_WINDOWS_H)
check_include_files("dirent.h"  FPHIG_HAVE_DIRENT_H)
check_include_files("libgen.h"  FPHIG_HAVE_LIBGEN_H)
check_include_files("pthread.h" FPHIG_HAVE_PTHREAD_H)
check_include_files("unistd.h"  FPHIG_HAVE_UNISTD_H)

include(CheckFunctionExists)

check_function_exists(strnstr FPHIG_HAVE_STRNSTR)

configure_file( fphig_config.cmake.in fphig_config.h )
include_directories( ${fphig_BINARY_DIR} )

set( CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )

set(FPHIG_INCLUDE ${fphig_SOURCE_DIR}/include)
set(FPHIG_SRC ${fphig_SOURCE_DIR}/src)
set(FPHIG_TEST ${fphig_SOURCE_DIR}/test)


if( ${BUILD_TESTING} )
    enable_testing()
endif()


include_directories(${CMAKE_CURRENT_BINARY_DIR}/include )
# TODO RPATH HANDLING
# https://gitlab.kitware.com/cmake/community/-/wikis/doc/cmake/RPATH-handling
# Currently the below setting will use absolute path for RPATH, when doing an install package this needs to be dynamic
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

add_subdirectory("include")

add_subdirectory("src")

add_subdirectory("test")

# ctags
find_program( FPHIG_CTAGS NAMES ctags )
if( FPHIG_CTAGS )
    execute_process( COMMAND ${FPHIG_CTAGS} --version
        RESULT_VARIABLE CTAGS_RETURN
        OUTPUT_QUIET
        ERROR_QUIET
    )
if( CTAGS_RETURN EQUAL 0 )
        add_custom_target( ctags ALL )
        add_custom_command( TARGET ctags
            POST_BUILD
            COMMAND ${FPHIG_CTAGS} -R --exclude=.git ${fphig_SOURCE_DIR}/src ${fphig_SOURCE_DIR}/test/mock
            WORKING_DIRECTORY ${fphig_SOURCE_DIR}
            COMMENT "running ctags"
        )
    else()
        message( WARNING "ctags is not Exuberant ctags" )
    endif( CTAGS_RETURN EQUAL 0 )
else()
    message( WARNING "ctags not found" )
endif( FPHIG_CTAGS )
