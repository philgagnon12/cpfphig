#ifndef FPHIG_THREAD_POOL_H
#define FPHIG_THREAD_POOL_H

#include "melphig/melphig.h"
#include "melphig/list.h"
#include "melphig/mutex.h"

struct fphig_thread_pool
{
    struct fphig_list                   thread_pool_threads;
    struct fphig_mutex                  mutex;
};

#define FPHIG_CONST_MPHIG_THREAD_POOL { \
    FPHIG_CONST_MPHIG_LIST,   \
    FPHIG_CONST_MPHIG_MUTEX   \
}

#endif
