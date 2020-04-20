# cmake .. -DBUILD_TESTING=YES -DCMAKE_BUILD_TYPE=Debug -DCMAKE_COMPILER_IS_GNUCXX=YES -DCOVERAGE=YES -DMELPHIG_WITH_FLUIDSYNTH=YES -DMELPHIG_WITH_LUA=YES
#powershell new-item -itemtype symboliclink -path soemthing -value somtheingelse

# TODO add warning flags to compiler

cmake_minimum_required(VERSION 3.13.2)

project( melphig
         VERSION 0.1
         LANGUAGES C )

option(MELPHIG_WITH_FLUIDSYNTH  "Input_Driver and Output_Driver" OFF)
option(MELPHIG_WITH_LUA         "Lua plugin" OFF)

# Default values
set(MELPHIG_CONST_MPHIG_DEFAULT_BPM             "120"     CACHE STRING "default bpm when initialized")
set(MELPHIG_CONST_MPHIG_DEFAULT_SILENCE_VALUE   "1.0"     CACHE STRING "default silence value when initialized")

set(MELPHIG_CONST_MPHIG_PARSED_NOTE_DEFAULT_KEY         "C"     CACHE STRING "struct mphig_parsed_note default value for member key")
set(MELPHIG_CONST_MPHIG_PARSED_NOTE_DEFAULT_OCTAVE      "5"     CACHE STRING "struct mphig_parsed_note default value for member octave")
set(MELPHIG_CONST_MPHIG_PARSED_NOTE_DEFAULT_VALUE       "1.0"   CACHE STRING "struct mphig_parsed_note default value for member value")
set(MELPHIG_CONST_MPHIG_PARSED_NOTE_DEFAULT_SIGNATURE   "0"     CACHE STRING "struct mphig_parsed_note default value for member signature")
set(MELPHIG_CONST_MPHIG_PARSED_NOTE_DEFAULT_VELOCITY    "60"    CACHE STRING "struct mphig_parsed_note default value for member velocity")

set(MELPHIG_CONST_MPHIG_NOTE_DEFAULT_KEY         "60"    CACHE STRING "struct mphig_note default value for member key")
set(MELPHIG_CONST_MPHIG_NOTE_DEFAULT_VALUE       "1.0"   CACHE STRING "struct mphig_note default value for member value")
set(MELPHIG_CONST_MPHIG_NOTE_DEFAULT_VELOCITY    "60"    CACHE STRING "struct mphig_note default value for member velocity")


include(CheckIncludeFiles)

check_include_files("dlfcn.h"   MELPHIG_HAVE_DLFCN_H)
check_include_files("windows.h" MELPHIG_HAVE_WINDOWS_H)
check_include_files("dirent.h"  MELPHIG_HAVE_DIRENT_H)
check_include_files("libgen.h"  MELPHIG_HAVE_LIBGEN_H)
check_include_files("pthread.h" MELPHIG_HAVE_PTHREAD_H)
check_include_files("unistd.h"  MELPHIG_HAVE_UNISTD_H)

include(CheckFunctionExists)

check_function_exists(strnstr MELPHIG_HAVE_STRNSTR)

configure_file( melphig_config.cmake.in melphig_config.h )
include_directories( ${melphig_BINARY_DIR} )

set( CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )

set(MELPHIG_INCLUDE ${melphig_SOURCE_DIR}/include)
set(MELPHIG_SRC ${melphig_SOURCE_DIR}/src)
set(MELPHIG_TEST ${melphig_SOURCE_DIR}/test)


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
find_program( MELPHIG_CTAGS NAMES ctags )
if( MELPHIG_CTAGS )
    execute_process( COMMAND ${MELPHIG_CTAGS} --version
        RESULT_VARIABLE CTAGS_RETURN
        OUTPUT_QUIET
        ERROR_QUIET
    )
if( CTAGS_RETURN EQUAL 0 )
        add_custom_target( ctags ALL )
        add_custom_command( TARGET ctags
            POST_BUILD
            COMMAND ${MELPHIG_CTAGS} -R --exclude=.git ${melphig_SOURCE_DIR}/src ${melphig_SOURCE_DIR}/test/mock
            WORKING_DIRECTORY ${melphig_SOURCE_DIR}
            COMMENT "running ctags"
        )
    else()
        message( WARNING "ctags is not Exuberant ctags" )
    endif( CTAGS_RETURN EQUAL 0 )
else()
    message( WARNING "ctags not found" )
endif( MELPHIG_CTAGS )
