include(FetchContent)

FetchContent_Declare( cmocka
    GIT_REPOSITORY "https://git.cryptomilk.org/projects/cmocka.git"
    GIT_TAG        "cmocka-1.1.5"
)

FetchContent_GetProperties( cmocka )

if( NOT cmocka_POPULATED )
    FetchContent_Populate( cmocka )
    FetchContent_GetProperties( cmocka )

    add_custom_command( OUTPUT "${cmocka_BINARY_DIR}/CMakeCache.txt"
        COMMAND ${CMAKE_COMMAND} ${cmocka_SOURCE_DIR} -DCMAKE_INSTALL_PREFIX="${cmocka_BINARY_DIR}" -DCMAKE_GENERATOR="${CMAKE_GENERATOR}" -DBUILD_SHARED_LIBS=YES
        WORKING_DIRECTORY ${cmocka_BINARY_DIR}
        COMMENT "configure cmocka"
    )

    add_custom_target( configurelibcmocka
        DEPENDS "${cmocka_BINARY_DIR}/CMakeCache.txt"
    )

add_custom_command( OUTPUT "${cmocka_BINARY_DIR}/lib/${CMAKE_LINK_LIBRARY_PREFIX}cmocka${CMAKE_LINK_LIBRARY_SUFFIX}"
        COMMAND ${CMAKE_COMMAND} --build ${cmocka_BINARY_DIR} --target install
        WORKING_DIRECTORY ${cmocka_BINARY_DIR}
        COMMENT "build libcmocka"
    )

    add_custom_target( buildlibcmocka
        DEPENDS "${cmocka_BINARY_DIR}/lib/${CMAKE_LINK_LIBRARY_PREFIX}cmocka${CMAKE_LINK_LIBRARY_SUFFIX}"
    )

    add_library(libcmocka SHARED IMPORTED GLOBAL)
    set_target_properties(libcmocka PROPERTIES IMPORTED_LOCATION "${cmocka_BINARY_DIR}/lib/${CMAKE_LINK_LIBRARY_PREFIX}cmocka${CMAKE_SHARED_LIBRARY_SUFFIX}")
    # Next line applies to windows only
    set_target_properties(libcmocka PROPERTIES IMPORTED_IMPLIB "${cmocka_BINARY_DIR}/lib/${CMAKE_LINK_LIBRARY_PREFIX}cmocka${CMAKE_LINK_LIBRARY_SUFFIX}")
    add_dependencies( buildlibcmocka configurelibcmocka )
    add_dependencies( libcmocka buildlibcmocka )
endif()

set(${CMAKE_FIND_PACKAGE_NAME}_INCLUDE_DIR "${cmocka_BINARY_DIR}/include" )
set(${CMAKE_FIND_PACKAGE_NAME}_LIBRARY     libcmocka )

# Windows start
if( EXISTS "${cmocka_BINARY_DIR}/bin/${CMAKE_LINK_LIBRARY_PREFIX}cmocka${CMAKE_SHARED_LIBRARY_SUFFIX}" )
    set(${CMAKE_FIND_PACKAGE_NAME}_RUNTIME_DLL "${cmocka_BINARY_DIR}/bin/${CMAKE_LINK_LIBRARY_PREFIX}cmocka${CMAKE_SHARED_LIBRARY_SUFFIX}" )
endif()
# Windows end

mark_as_advanced(${CMAKE_FIND_PACKAGE_NAME}_INCLUDE_DIR ${CMAKE_FIND_PACKAGE_NAME}_LIBRARY ${CMAKE_FIND_PACKAGE_NAME}_RUNTIME_DLL)


set( ${CMAKE_FIND_PACKAGE_NAME}_LIBRARIES    ${${CMAKE_FIND_PACKAGE_NAME}_LIBRARY})
set( ${CMAKE_FIND_PACKAGE_NAME}_INCLUDE_DIRS ${${CMAKE_FIND_PACKAGE_NAME}_INCLUDE_DIR})
set( ${CMAKE_FIND_PACKAGE_NAME}_RUNTIME_DLLS ${${CMAKE_FIND_PACKAGE_NAME}_RUNTIME_DLL})

# set(${CMAKE_FIND_PACKAGE_NAME}_FOUND TRUE)
# 
# mark_as_advanced(${CMAKE_FIND_PACKAGE_NAME}_FOUND)

