# Symlink for header files
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/fphig
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/../src ${CMAKE_CURRENT_BINARY_DIR}/fphig
    COMMENT "symlink include/fphig"
)

add_custom_target( symlink_include_fphig ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/fphig
)

