#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_PTHREAD_H

#include "cpfphig/thread_cond_broadcast.h"
#include "cpfphig/thread_cond.h"

#include <pthread.h>

cpfphig
cpfphig_thread_cond_broadcast( struct cpfphig_thread_cond*             Cond,
                             CPFPHIG_OPTIONAL struct cpfphig_error*  Error )
{
    // NULL checks
    if( Cond == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Cond is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    if( 0 != pthread_cond_broadcast( Cond->pthread_cond ) )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "pthread_cond_broadcast failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    return CPFPHIG_OK;
}

#endif
