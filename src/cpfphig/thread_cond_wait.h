#ifndef CPFPHIG_THREAD_COND_WAIT_H
#define CPFPHIG_THREAD_COND_WAIT_H

#include "cpfphig/cpfphig.h"
#include "cpfphig/thread_cond.h"
#include "cpfphig/mutex.h"

cpfphig
cpfphig_thread_cond_wait( struct cpfphig_thread_cond*               Cond,
                        struct cpfphig_mutex*                     Mutex,
                        CPFPHIG_OPTIONAL struct cpfphig_error*    Error );

#endif
