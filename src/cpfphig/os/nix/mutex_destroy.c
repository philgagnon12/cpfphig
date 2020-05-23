#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_PTHREAD_H

#include "cpfphig/mutex_destroy.h"
#include "cpfphig/mutex.h"
#include "cpfphig/free.h"

#include <pthread.h>

cpfphig
cpfphig_mutex_destroy( struct cpfphig_mutex*                    Mutex,
                       CPFPHIG_OPTIONAL struct cpfphig_error*   Error )
{
    // NULL checks
    if( Mutex == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Mutex is NULL", Error );

        return CPFPHIG_FAIL;
    }

    if( 0 != pthread_mutex_destroy( Mutex->pthread_mutex ) )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "pthread_mutex_destroy failed", Error );

        return CPFPHIG_FAIL;
    }

    return cpfphig_free( &Mutex->pthread_mutex,
                         Error );
}

#endif
