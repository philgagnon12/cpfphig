#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_PTHREAD_H

#include "cpfphig/cpfphig.h"
#include "cpfphig/thread_cond.h"
#include "cpfphig/mutex.h"

#include <pthread.h>

cpfphig
cpfphig_thread_cond_wait( struct cpfphig_thread_cond*               Cond,
                        struct cpfphig_mutex*                     Mutex,
                        CPFPHIG_OPTIONAL struct cpfphig_error*    Error )
{
    // NULL checks
    if( Cond == NULL || Mutex == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Cond or Mutex is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    if( 0 != pthread_cond_wait( Cond->pthread_cond,
                                Mutex->pthread_mutex ) )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "pthread_cond_wait failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    return CPFPHIG_OK;
}

#endif
