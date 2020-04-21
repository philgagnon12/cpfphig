#ifndef CPFPHIG_THREAD_POOL_THREAD_H
#define CPFPHIG_THREAD_POOL_THREAD_H

#include "cpfphig/cpfphig.h"
#include "cpfphig/publisher.h"
#include "cpfphig/thread.h"
#include "cpfphig/thread_cond.h"
#include "cpfphig/mutex.h"

typedef void (cpfphig_thread_pool_thread_routine_symbol)(void*);

enum cpfphig_thread_pool_thread_cond_kind
{
    cpfphig_thread_pool_thread_cond_kind_ready,
    cpfphig_thread_pool_thread_cond_kind_routine,
    cpfphig_thread_pool_thread_cond_kind_abort,
};

struct cpfphig_thread_pool_thread
{
    struct cpfphig_publisher*                     publisher;

    struct cpfphig_thread                         thread;
    struct cpfphig_thread_cond                    thread_cond;
    enum cpfphig_thread_pool_thread_cond_kind     thread_cond_kind;
    struct cpfphig_thread_cond                    ready_thread_cond;
    struct cpfphig_mutex                          mutex;

    cpfphig_thread_pool_thread_routine_symbol*    routine;
    void*                                       routine_arg;

    struct cpfphig_mutex                          busy_mutex;
    int                                         busy;
};

#define CPFPHIG_CONST_CPFPHIG_THREAD_POOL_THREAD { \
    NULL,                                       \
    CPFPHIG_CONST_CPFPHIG_THREAD,                 \
    CPFPHIG_CONST_CPFPHIG_THREAD_COND,            \
    cpfphig_thread_pool_thread_cond_kind_ready,   \
    CPFPHIG_CONST_CPFPHIG_THREAD_COND,            \
    CPFPHIG_CONST_CPFPHIG_MUTEX,                  \
    NULL,                                       \
    NULL,                                       \
    CPFPHIG_CONST_CPFPHIG_MUTEX,                  \
    0,                                          \
}

#endif
