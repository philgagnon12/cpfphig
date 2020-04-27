#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_PTHREAD_H

#include "cpfphig/thread_exit.h"

#include <pthread.h>

void
cpfphig_thread_exit( int Ret )
{
    pthread_exit( (void*)Ret );
}

#endif
