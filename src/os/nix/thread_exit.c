#include "melphig/melphig.h"

#ifdef MELPHIG_HAVE_PTHREAD_H

#include "melphig/thread_exit.h"

#include <pthread.h>

mphig
mphig_thread_exit( MELPHIG_OPTIONAL void*               Value_Ptr,
                   MELPHIG_OPTIONAL struct mphig_error* Error )
{
    pthread_exit( Value_Ptr );

    return MELPHIG_OK;
}

#endif
