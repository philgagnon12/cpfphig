if( NOT MINGW )
target_sources( ${CMAKE_PROJECT_NAME}
    PRIVATE
    library_load.c
    library_sym.c
    library_unload.c
    directory_list.c
    thread_create.c
    thread_join.c
    thread_exit.c
    mutex_init.c
    mutex_lock.c
    mutex_unlock.c
    mutex_destroy.c
    thread_cond_init.c
    thread_cond_destroy.c
    thread_cond_attr_init.c
    thread_cond_wait.c
    thread_cond_timed_wait.c
    thread_cond_signal.c
    thread_cond_broadcast.c
    thread_cond_attr_destroy.c
    sleep.c
    basename.c
    dirname.c
)
endif()