#ifndef FPHIG_DEFER_H
#define FPHIG_DEFER_H

#include "fphig/fphig.h"
#include "fphig/thread_pool.h"

typedef void (fphig_defer_routine_symbol)( void* Arg );

struct fphig_deferred
{
    fphig_defer_routine_symbol* routine;
    void*                       routine_arg;
    int                         delay_milliseconds;
};

#define FPHIG_CONST_MPHIG_DEFERRED { \
    NULL,   \
    NULL,   \
    0,      \
}

fphig
fphig_defer( fphig_defer_routine_symbol*            Routine,
             void*                                  Routine_Arg,
             int                                    Delay_Milliseconds,
             struct fphig_thread_pool*              Thread_Pool,
             FPHIG_OPTIONAL struct fphig_error*   Error );

#endif
