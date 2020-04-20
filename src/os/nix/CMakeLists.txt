list(APPEND SRC_LIST
     ${FPHIG_SRC}/os/nix/library_load.c
     ${FPHIG_SRC}/os/nix/library_sym.c
     ${FPHIG_SRC}/os/nix/library_unload.c
     ${FPHIG_SRC}/os/nix/directory_list.c
     ${FPHIG_SRC}/os/nix/thread_create.c
     ${FPHIG_SRC}/os/nix/thread_join.c
     ${FPHIG_SRC}/os/nix/thread_exit.c
     ${FPHIG_SRC}/os/nix/mutex_init.c
     ${FPHIG_SRC}/os/nix/mutex_lock.c
     ${FPHIG_SRC}/os/nix/mutex_unlock.c
     ${FPHIG_SRC}/os/nix/mutex_destroy.c
     ${FPHIG_SRC}/os/nix/thread_cond_init.c
     ${FPHIG_SRC}/os/nix/thread_cond_destroy.c
     ${FPHIG_SRC}/os/nix/thread_cond_attr_init.c
     ${FPHIG_SRC}/os/nix/thread_cond_wait.c
     ${FPHIG_SRC}/os/nix/thread_cond_timed_wait.c
     ${FPHIG_SRC}/os/nix/thread_cond_signal.c
     ${FPHIG_SRC}/os/nix/thread_cond_broadcast.c
     ${FPHIG_SRC}/os/nix/thread_cond_attr_destroy.c
     ${FPHIG_SRC}/os/nix/sleep.c
)
set( SRC_LIST ${SRC_LIST} PARENT_SCOPE)
