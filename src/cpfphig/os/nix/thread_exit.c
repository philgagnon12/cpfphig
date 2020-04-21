#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_PTHREAD_H

#include "cpfphig/thread_exit.h"

#include <pthread.h>

cpfphig
cpfphig_thread_exit( CPFPHIG_OPTIONAL void*               Value_Ptr,
                   CPFPHIG_OPTIONAL struct cpfphig_error* Error )
{
    pthread_exit( Value_Ptr );

    return CPFPHIG_OK;
}

#endif
