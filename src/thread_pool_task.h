#ifndef MELPHIG_THREAD_POOL_TASK_H
#define MELPHIG_THREAD_POOL_TASK_H

#include "melphig/melphig.h"
#include "melphig/thread_pool.h"
#include "melphig/thread_pool_thread.h"

mphig
mphig_thread_pool_task( struct mphig_thread_pool*                   Thread_Pool,
                        mphig_thread_pool_thread_routine_symbol*    Routine,
                        void*                                       Routine_Arg,
                        MELPHIG_OPTIONAL struct mphig_error*        Error );
#endif
