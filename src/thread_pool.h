#ifndef MELPHIG_THREAD_POOL_H
#define MELPHIG_THREAD_POOL_H

#include "melphig/melphig.h"
#include "melphig/list.h"
#include "melphig/mutex.h"

struct mphig_thread_pool
{
    struct mphig_list                   thread_pool_threads;
    struct mphig_mutex                  mutex;
};

#define MELPHIG_CONST_MPHIG_THREAD_POOL { \
    MELPHIG_CONST_MPHIG_LIST,   \
    MELPHIG_CONST_MPHIG_MUTEX   \
}

#endif
