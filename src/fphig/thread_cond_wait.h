#ifndef FPHIG_THREAD_COND_WAIT_H
#define FPHIG_THREAD_COND_WAIT_H

#include "fphig/fphig.h"
#include "fphig/thread_cond.h"
#include "fphig/mutex.h"

fphig
fphig_thread_cond_wait( struct fphig_thread_cond*               Cond,
                        struct fphig_mutex*                     Mutex,
                        FPHIG_OPTIONAL struct fphig_error*    Error );

#endif
