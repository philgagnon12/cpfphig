set( MELPHIG_HEADER "${CMAKE_CURRENT_BINARY_DIR}/melphig.h"  )
set( MELPHIG_HEADER ${MELPHIG_HEADER} PARENT_SCOPE )


# Symlink for header files
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/melphig
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/../src ${CMAKE_CURRENT_BINARY_DIR}/melphig
    COMMENT "symlink include/melphig"
)

add_custom_target( symlink_include_melphig ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/melphig
)

# Create public melphig.h
add_custom_command(
    OUTPUT ${MELPHIG_HEADER}
    COMMAND ${CMAKE_COMMAND} -D MELPHIG_HEADER=${MELPHIG_HEADER} -P ${CMAKE_MODULE_PATH}/MelphigHeader.cmake
)

add_custom_target(
    melphig_header ALL
    DEPENDS ${MELPHIG_HEADER}
)

