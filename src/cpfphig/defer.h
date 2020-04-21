#ifndef CPFPHIG_DEFER_H
#define CPFPHIG_DEFER_H

#include "cpfphig/cpfphig.h"
#include "cpfphig/thread_pool.h"

typedef void (cpfphig_defer_routine_symbol)( void* Arg );

struct cpfphig_deferred
{
    cpfphig_defer_routine_symbol* routine;
    void*                       routine_arg;
    int                         delay_milliseconds;
};

#define CPFPHIG_CONST_CPFPHIG_DEFERRED { \
    NULL,   \
    NULL,   \
    0,      \
}

cpfphig
cpfphig_defer( cpfphig_defer_routine_symbol*            Routine,
             void*                                  Routine_Arg,
             int                                    Delay_Milliseconds,
             struct cpfphig_thread_pool*              Thread_Pool,
             CPFPHIG_OPTIONAL struct cpfphig_error*   Error );

#endif
