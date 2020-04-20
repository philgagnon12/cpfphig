#include "fphig/fphig.h"

#ifdef FPHIG_HAVE_PTHREAD_H

#include "fphig/thread_exit.h"

#include <pthread.h>

fphig
fphig_thread_exit( FPHIG_OPTIONAL void*               Value_Ptr,
                   FPHIG_OPTIONAL struct fphig_error* Error )
{
    pthread_exit( Value_Ptr );

    return FPHIG_OK;
}

#endif
