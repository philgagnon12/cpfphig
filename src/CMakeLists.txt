set(
    HEADER_LIST
    ${FPHIG_SRC}/fphig.h
    ${FPHIG_SRC}/lexicon.h
    ${FPHIG_SRC}/malloc.h
    ${FPHIG_SRC}/free.h
    ${FPHIG_SRC}/list.h
    ${FPHIG_SRC}/library_load.h
    ${FPHIG_SRC}/library_sym.h
    ${FPHIG_SRC}/library_unload.h
    ${FPHIG_SRC}/modules_load_all.h
    ${FPHIG_SRC}/directory_list.h
    ${FPHIG_SRC}/destroy_directory_list.h
    ${FPHIG_SRC}/thread.h
    ${FPHIG_SRC}/thread_create.h
    ${FPHIG_SRC}/thread_join.h
    ${FPHIG_SRC}/thread_exit.h
    ${FPHIG_SRC}/thread_cond.h
    ${FPHIG_SRC}/thread_cond_init.h
    ${FPHIG_SRC}/thread_cond_destroy.h
    ${FPHIG_SRC}/thread_cond_attr.h
    ${FPHIG_SRC}/thread_cond_attr_init.h
    ${FPHIG_SRC}/thread_cond_attr_destroy.h
    ${FPHIG_SRC}/thread_cond_signal.h
    ${FPHIG_SRC}/thread_cond_broadcast.h
    ${FPHIG_SRC}/thread_cond_wait.h
    ${FPHIG_SRC}/mutex.h
    ${FPHIG_SRC}/mutex_attr.h
    ${FPHIG_SRC}/mutex_init.h
    ${FPHIG_SRC}/mutex_destroy.h
    ${FPHIG_SRC}/mutex_lock.h
    ${FPHIG_SRC}/mutex_unlock.h
    ${FPHIG_SRC}/assert.h
    ${FPHIG_SRC}/publisher.h
    ${FPHIG_SRC}/publisher_create.h
    ${FPHIG_SRC}/publisher_destroy.h
    ${FPHIG_SRC}/publisher_subscribe.h
    ${FPHIG_SRC}/publisher_unsubscribe.h
    ${FPHIG_SRC}/publisher_publish.h
    ${FPHIG_SRC}/subscription.h
    ${FPHIG_SRC}/thread_pool.h
    ${FPHIG_SRC}/thread_pool_thread.h
    ${FPHIG_SRC}/thread_pool_create.h
    ${FPHIG_SRC}/thread_pool_thread_create.h
    ${FPHIG_SRC}/thread_pool_task.h
    ${FPHIG_SRC}/destroy_thread_pool_thread.h
    ${FPHIG_SRC}/destroy_thread_pool.h
    ${FPHIG_SRC}/strnstr.h
    ${FPHIG_SRC}/defer.h
    ${FPHIG_SRC}/sleep.h
)


set(
    SRC_LIST
    ${FPHIG_SRC}/fphig.c
    ${FPHIG_SRC}/malloc.c
    ${FPHIG_SRC}/free.c
    ${FPHIG_SRC}/free.h
    ${FPHIG_SRC}/list.h
    ${FPHIG_SRC}/list_push.c
    ${FPHIG_SRC}/list_shift.c
    ${FPHIG_SRC}/list_next.c
    ${FPHIG_SRC}/list_remove.c
    ${FPHIG_SRC}/list_copy.c
    ${FPHIG_SRC}/modules_load_all.c
    ${FPHIG_SRC}/destroy_directory_list.c
    ${FPHIG_SRC}/assert.c
    ${FPHIG_SRC}/publisher_create.c
    ${FPHIG_SRC}/publisher_destroy.c
    ${FPHIG_SRC}/publisher_subscribe.c
    ${FPHIG_SRC}/publisher_unsubscribe.c
    ${FPHIG_SRC}/publisher_publish.c
    ${FPHIG_SRC}/thread_pool_create.c
    ${FPHIG_SRC}/thread_pool_thread_create.c
    ${FPHIG_SRC}/thread_pool_task.c
    ${FPHIG_SRC}/destroy_thread_pool_thread.c
    ${FPHIG_SRC}/destroy_thread_pool.c
    ${FPHIG_SRC}/defer.c
)


add_subdirectory("os")

add_library( fphig ${SRC_LIST} )

set( HEADER_LIST ${HEADER_LIST} PARENT_SCOPE )
set( SRC_LIST ${SRC_LIST} PARENT_SCOPE )
