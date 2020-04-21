#ifndef CPFPHIG_THREAD_POOL_H
#define CPFPHIG_THREAD_POOL_H

#include "cpfphig/cpfphig.h"
#include "cpfphig/list.h"
#include "cpfphig/mutex.h"

struct cpfphig_thread_pool
{
    struct cpfphig_list                   thread_pool_threads;
    struct cpfphig_mutex                  mutex;
};

#define CPFPHIG_CONST_CPFPHIG_THREAD_POOL { \
    CPFPHIG_CONST_CPFPHIG_LIST,   \
    CPFPHIG_CONST_CPFPHIG_MUTEX   \
}

#endif
