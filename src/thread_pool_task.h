#ifndef FPHIG_THREAD_POOL_TASK_H
#define FPHIG_THREAD_POOL_TASK_H

#include "melphig/melphig.h"
#include "melphig/thread_pool.h"
#include "melphig/thread_pool_thread.h"

fphig
fphig_thread_pool_task( struct fphig_thread_pool*                   Thread_Pool,
                        fphig_thread_pool_thread_routine_symbol*    Routine,
                        void*                                       Routine_Arg,
                        FPHIG_OPTIONAL struct fphig_error*        Error );
#endif
