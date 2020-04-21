
add_library( ${CMAKE_PROJECT_NAME} )

add_subdirectory("os/linux")
add_subdirectory("os/nix")
add_subdirectory("os/unix")

set_property( TARGET ${CMAKE_PROJECT_NAME}
    PROPERTY PUBLIC_HEADER
    cpfphig.h
    malloc.h
    free.h
    list.h
    library_load.h
    library_sym.h
    library_unload.h
    modules_load_all.h
    directory_list.h
    destroy_directory_list.h
    thread.h
    thread_create.h
    thread_join.h
    thread_exit.h
    thread_cond.h
    thread_cond_init.h
    thread_cond_destroy.h
    thread_cond_attr.h
    thread_cond_attr_init.h
    thread_cond_attr_destroy.h
    thread_cond_signal.h
    thread_cond_broadcast.h
    thread_cond_wait.h
    mutex.h
    mutex_attr.h
    mutex_init.h
    mutex_destroy.h
    mutex_lock.h
    mutex_unlock.h
    assert.h
    publisher.h
    publisher_create.h
    publisher_destroy.h
    publisher_subscribe.h
    publisher_unsubscribe.h
    publisher_publish.h
    subscription.h
    thread_pool.h
    thread_pool_thread.h
    thread_pool_create.h
    thread_pool_thread_create.h
    thread_pool_task.h
    destroy_thread_pool_thread.h
    destroy_thread_pool.h
    strnstr.h
    defer.h
    sleep.h
)

target_sources( ${CMAKE_PROJECT_NAME}
    PRIVATE
    cpfphig.c
    malloc.c
    free.c
    free.h
    list.h
    list_push.c
    list_shift.c
    list_next.c
    list_remove.c
    list_copy.c
    modules_load_all.c
    destroy_directory_list.c
    assert.c
    publisher_create.c
    publisher_destroy.c
    publisher_subscribe.c
    publisher_unsubscribe.c
    publisher_publish.c
    thread_pool_create.c
    thread_pool_thread_create.c
    thread_pool_task.c
    destroy_thread_pool_thread.c
    destroy_thread_pool.c
    defer.c
)

find_package(Threads REQUIRED)

# expected to be empty on macosx
if( NOT CMAKE_DL_LIBS )
    message( WARNING "dl lib not found" )
endif()

target_link_libraries( ${CMAKE_PROJECT_NAME} Threads::Threads ${CMAKE_DL_LIBS} )

set_target_properties( ${CMAKE_PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

set_target_properties( ${CMAKE_PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

# TODO have a Release , DEbug ReleaseWithDebugInfo
# install(TARGETS target
#         CONFIGURATIONS Debug
#         RUNTIME DESTINATION Debug/bin)

target_include_directories( ${CMAKE_PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include>
)

include(GNUInstallDirs)

install( TARGETS ${CMAKE_PROJECT_NAME}
    EXPORT ${CMAKE_PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME}
)

install( EXPORT ${CMAKE_PROJECT_NAME}
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)
