list(APPEND SRC_LIST
     ${MELPHIG_SRC}/os/nix/library_load.c
     ${MELPHIG_SRC}/os/nix/library_sym.c
     ${MELPHIG_SRC}/os/nix/library_unload.c
     ${MELPHIG_SRC}/os/nix/directory_list.c
     ${MELPHIG_SRC}/os/nix/thread_create.c
     ${MELPHIG_SRC}/os/nix/thread_join.c
     ${MELPHIG_SRC}/os/nix/thread_exit.c
     ${MELPHIG_SRC}/os/nix/mutex_init.c
     ${MELPHIG_SRC}/os/nix/mutex_lock.c
     ${MELPHIG_SRC}/os/nix/mutex_unlock.c
     ${MELPHIG_SRC}/os/nix/mutex_destroy.c
     ${MELPHIG_SRC}/os/nix/thread_cond_init.c
     ${MELPHIG_SRC}/os/nix/thread_cond_destroy.c
     ${MELPHIG_SRC}/os/nix/thread_cond_attr_init.c
     ${MELPHIG_SRC}/os/nix/thread_cond_wait.c
     ${MELPHIG_SRC}/os/nix/thread_cond_timed_wait.c
     ${MELPHIG_SRC}/os/nix/thread_cond_signal.c
     ${MELPHIG_SRC}/os/nix/thread_cond_broadcast.c
     ${MELPHIG_SRC}/os/nix/thread_cond_attr_destroy.c
     ${MELPHIG_SRC}/os/nix/sleep.c
)
set( SRC_LIST ${SRC_LIST} PARENT_SCOPE)
