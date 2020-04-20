#ifndef MELPHIG_THREAD_COND_WAIT_H
#define MELPHIG_THREAD_COND_WAIT_H

#include "melphig/melphig.h"
#include "melphig/thread_cond.h"
#include "melphig/mutex.h"

mphig
mphig_thread_cond_wait( struct mphig_thread_cond*               Cond,
                        struct mphig_mutex*                     Mutex,
                        MELPHIG_OPTIONAL struct mphig_error*    Error );

#endif
