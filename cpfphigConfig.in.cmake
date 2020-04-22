@PACKAGE_INIT@

include(CMakeFindDependencyMacro)

find_dependency( Threads )

include(@CMAKE_CURRENT_BINARY_DIR@/@CMAKE_PROJECT_NAME@.cmake)
