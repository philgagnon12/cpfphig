#ifndef FPHIG_THREAD_POOL_H
#define FPHIG_THREAD_POOL_H

#include "fphig/fphig.h"
#include "fphig/list.h"
#include "fphig/mutex.h"

struct fphig_thread_pool
{
    struct fphig_list                   thread_pool_threads;
    struct fphig_mutex                  mutex;
};

#define FPHIG_CONST_FPHIG_THREAD_POOL { \
    FPHIG_CONST_FPHIG_LIST,   \
    FPHIG_CONST_FPHIG_MUTEX   \
}

#endif
