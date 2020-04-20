#ifndef MELPHIG_DEFER_H
#define MELPHIG_DEFER_H

#include "melphig/melphig.h"
#include "melphig/thread_pool.h"

typedef void (mphig_defer_routine_symbol)( void* Arg );

struct mphig_deferred
{
    mphig_defer_routine_symbol* routine;
    void*                       routine_arg;
    int                         delay_milliseconds;
};

#define MELPHIG_CONST_MPHIG_DEFERRED { \
    NULL,   \
    NULL,   \
    0,      \
}

mphig
mphig_defer( mphig_defer_routine_symbol*            Routine,
             void*                                  Routine_Arg,
             int                                    Delay_Milliseconds,
             struct mphig_thread_pool*              Thread_Pool,
             MELPHIG_OPTIONAL struct mphig_error*   Error );

#endif
