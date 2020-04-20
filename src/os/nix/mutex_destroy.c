#include "melphig/melphig.h"

#ifdef MELPHIG_HAVE_PTHREAD_H

#include "melphig/mutex_destroy.h"
#include "melphig/mutex.h"
#include "melphig/free.h"

#include <pthread.h>

mphig
mphig_mutex_destroy( struct mphig_mutex*                    Mutex,
                     MELPHIG_OPTIONAL struct mphig_error*   Error )
{
    // NULL checks
    if( Mutex == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Mutex is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    if( 0 != pthread_mutex_destroy( Mutex->pthread_mutex ) )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "pthread_mutex_destroy failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    return mphig_free( &Mutex->pthread_mutex,
                       Error );
}

#endif
