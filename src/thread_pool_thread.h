#ifndef MELPHIG_THREAD_POOL_THREAD_H
#define MELPHIG_THREAD_POOL_THREAD_H

#include "melphig/melphig.h"
#include "melphig/publisher.h"
#include "melphig/thread.h"
#include "melphig/thread_cond.h"
#include "melphig/mutex.h"

typedef void (mphig_thread_pool_thread_routine_symbol)(void*);

enum mphig_thread_pool_thread_cond_kind
{
    mphig_thread_pool_thread_cond_kind_ready,
    mphig_thread_pool_thread_cond_kind_routine,
    mphig_thread_pool_thread_cond_kind_abort,
};

struct mphig_thread_pool_thread
{
    struct mphig_publisher*                     publisher;

    struct mphig_thread                         thread;
    struct mphig_thread_cond                    thread_cond;
    enum mphig_thread_pool_thread_cond_kind     thread_cond_kind;
    struct mphig_thread_cond                    ready_thread_cond;
    struct mphig_mutex                          mutex;

    mphig_thread_pool_thread_routine_symbol*    routine;
    void*                                       routine_arg;

    struct mphig_mutex                          busy_mutex;
    int                                         busy;
};

#define MELPHIG_CONST_MPHIG_THREAD_POOL_THREAD { \
    NULL,                                       \
    MELPHIG_CONST_MPHIG_THREAD,                 \
    MELPHIG_CONST_MPHIG_THREAD_COND,            \
    mphig_thread_pool_thread_cond_kind_ready,   \
    MELPHIG_CONST_MPHIG_THREAD_COND,            \
    MELPHIG_CONST_MPHIG_MUTEX,                  \
    NULL,                                       \
    NULL,                                       \
    MELPHIG_CONST_MPHIG_MUTEX,                  \
    0,                                          \
}

#endif
