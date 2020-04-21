#ifndef CPFPHIG_THREAD_POOL_TASK_H
#define CPFPHIG_THREAD_POOL_TASK_H

#include "cpfphig/cpfphig.h"
#include "cpfphig/thread_pool.h"
#include "cpfphig/thread_pool_thread.h"

cpfphig
cpfphig_thread_pool_task( struct cpfphig_thread_pool*                   Thread_Pool,
                        cpfphig_thread_pool_thread_routine_symbol*    Routine,
                        void*                                       Routine_Arg,
                        CPFPHIG_OPTIONAL struct cpfphig_error*        Error );
#endif
