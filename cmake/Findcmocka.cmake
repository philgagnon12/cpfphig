include(FetchContent)

if( NOT EXISTS "${PROJECT_SOURCE_DIR}/vendor/cmocka" )
    FetchContent_Declare( cmocka
        GIT_REPOSITORY "https://git.cryptomilk.org/projects/cmocka.git"
        GIT_TAG        "cmocka-1.1.5"
        SOURCE_DIR     "${PROJECT_SOURCE_DIR}/vendor/cmocka"
    )
    message( INFO "Downloaded https://git.cryptomilk.org/projects/cmocka.git" )
else()
    FetchContent_Declare( cmocka
        URL            "${PROJECT_SOURCE_DIR}/vendor/cmocka"
    )
endif()

FetchContent_GetProperties( cmocka )

if(WIN32)
    set(LIBRARY_PREFIX "")
    set(LIBRARY_SUFFIX ${CMAKE_LINK_LIBRARY_SUFFIX})
else()
    set(LIBRARY_PREFIX ${CMAKE_SHARED_LIBRARY_PREFIX})
    set(LIBRARY_SUFFIX ${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()

if( NOT cmocka_POPULATED )
    FetchContent_Populate( cmocka )
    FetchContent_GetProperties( cmocka )

    set( LIBRARY_PATH "${cmocka_BINARY_DIR}/lib/${LIBRARY_PREFIX}cmocka${LIBRARY_SUFFIX}" )

    add_custom_command( OUTPUT "${cmocka_BINARY_DIR}/CMakeCache.txt"
        COMMAND ${CMAKE_COMMAND} ${cmocka_SOURCE_DIR} -G "${CMAKE_GENERATOR}" -DCMAKE_INSTALL_PREFIX="${cmocka_BINARY_DIR}" -DBUILD_SHARED_LIBS=YES
        WORKING_DIRECTORY ${cmocka_BINARY_DIR}
        COMMENT "configure cmocka"
    )

    add_custom_target( configurelibcmocka
        DEPENDS "${cmocka_BINARY_DIR}/CMakeCache.txt"
    )

    add_custom_command( OUTPUT "${LIBRARY_PATH}"
        COMMAND ${CMAKE_COMMAND} --build ${cmocka_BINARY_DIR} --target install
        WORKING_DIRECTORY ${cmocka_BINARY_DIR}
        COMMENT "build libcmocka"
    )

    add_custom_target( buildlibcmocka
        DEPENDS "${LIBRARY_PATH}"
    )

    add_library(libcmocka SHARED IMPORTED GLOBAL)
    set_target_properties(libcmocka PROPERTIES IMPORTED_LOCATION "${LIBRARY_PATH}")
    # Next line applies to windows only
    set_target_properties(libcmocka PROPERTIES IMPORTED_IMPLIB "${LIBRARY_PATH}")
    add_dependencies( buildlibcmocka configurelibcmocka )
    add_dependencies( libcmocka buildlibcmocka )
endif()

set(${CMAKE_FIND_PACKAGE_NAME}_INCLUDE_DIR "${cmocka_BINARY_DIR}/include" )
set(${CMAKE_FIND_PACKAGE_NAME}_LIBRARY     libcmocka )

# Windows start
if( EXISTS "${cmocka_BINARY_DIR}/bin/${LIBRARY_PREFIX}cmocka${CMAKE_SHARED_LIBRARY_SUFFIX}" )
    set(${CMAKE_FIND_PACKAGE_NAME}_RUNTIME_DLL "${cmocka_BINARY_DIR}/bin/${LIBRARY_PREFIX}cmocka${CMAKE_SHARED_LIBRARY_SUFFIX}" )
endif()
# Windows end

mark_as_advanced(${CMAKE_FIND_PACKAGE_NAME}_INCLUDE_DIR ${CMAKE_FIND_PACKAGE_NAME}_LIBRARY ${CMAKE_FIND_PACKAGE_NAME}_RUNTIME_DLL)


set( ${CMAKE_FIND_PACKAGE_NAME}_LIBRARIES    ${${CMAKE_FIND_PACKAGE_NAME}_LIBRARY})
set( ${CMAKE_FIND_PACKAGE_NAME}_INCLUDE_DIRS ${${CMAKE_FIND_PACKAGE_NAME}_INCLUDE_DIR})
set( ${CMAKE_FIND_PACKAGE_NAME}_RUNTIME_DLLS ${${CMAKE_FIND_PACKAGE_NAME}_RUNTIME_DLL})

# set(${CMAKE_FIND_PACKAGE_NAME}_FOUND TRUE)
# 
# mark_as_advanced(${CMAKE_FIND_PACKAGE_NAME}_FOUND)

