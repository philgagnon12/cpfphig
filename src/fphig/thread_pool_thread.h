#ifndef FPHIG_THREAD_POOL_THREAD_H
#define FPHIG_THREAD_POOL_THREAD_H

#include "fphig/fphig.h"
#include "fphig/publisher.h"
#include "fphig/thread.h"
#include "fphig/thread_cond.h"
#include "fphig/mutex.h"

typedef void (fphig_thread_pool_thread_routine_symbol)(void*);

enum fphig_thread_pool_thread_cond_kind
{
    fphig_thread_pool_thread_cond_kind_ready,
    fphig_thread_pool_thread_cond_kind_routine,
    fphig_thread_pool_thread_cond_kind_abort,
};

struct fphig_thread_pool_thread
{
    struct fphig_publisher*                     publisher;

    struct fphig_thread                         thread;
    struct fphig_thread_cond                    thread_cond;
    enum fphig_thread_pool_thread_cond_kind     thread_cond_kind;
    struct fphig_thread_cond                    ready_thread_cond;
    struct fphig_mutex                          mutex;

    fphig_thread_pool_thread_routine_symbol*    routine;
    void*                                       routine_arg;

    struct fphig_mutex                          busy_mutex;
    int                                         busy;
};

#define FPHIG_CONST_FPHIG_THREAD_POOL_THREAD { \
    NULL,                                       \
    FPHIG_CONST_FPHIG_THREAD,                 \
    FPHIG_CONST_FPHIG_THREAD_COND,            \
    fphig_thread_pool_thread_cond_kind_ready,   \
    FPHIG_CONST_FPHIG_THREAD_COND,            \
    FPHIG_CONST_FPHIG_MUTEX,                  \
    NULL,                                       \
    NULL,                                       \
    FPHIG_CONST_FPHIG_MUTEX,                  \
    0,                                          \
}

#endif
