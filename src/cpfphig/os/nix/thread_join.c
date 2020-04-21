#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_PTHREAD_H

#include "cpfphig/thread_join.h"
#include "cpfphig/thread.h"

#include <pthread.h>

cpfphig
cpfphig_thread_join( struct cpfphig_thread*                 Thread,
                   CPFPHIG_OPTIONAL void**              Value_Ptr,
                   CPFPHIG_OPTIONAL struct cpfphig_error* Error )
{
    // NULL checks
    if( Thread == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Thread is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    if( 0 != pthread_join( Thread->pthread,
                           Value_Ptr ) )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "pthread_join failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    return CPFPHIG_OK;
}

#endif
