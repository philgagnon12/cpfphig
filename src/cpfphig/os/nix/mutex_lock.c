#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_PTHREAD_H

#include "cpfphig/mutex_lock.h"
#include "cpfphig/mutex.h"

#include <pthread.h>

cpfphig
cpfphig_mutex_lock( struct cpfphig_mutex*                    Mutex,
                  CPFPHIG_OPTIONAL struct cpfphig_error*   Error )
{
    // NULL checks
    if( Mutex == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Mutex is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    if( 0 != pthread_mutex_lock( Mutex->pthread_mutex ) )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "pthread_mutex_lock failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    return CPFPHIG_OK;
}

#endif
