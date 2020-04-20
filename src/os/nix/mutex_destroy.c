#include "melphig/melphig.h"

#ifdef FPHIG_HAVE_PTHREAD_H

#include "melphig/mutex_destroy.h"
#include "melphig/mutex.h"
#include "melphig/free.h"

#include <pthread.h>

fphig
fphig_mutex_destroy( struct fphig_mutex*                    Mutex,
                     FPHIG_OPTIONAL struct fphig_error*   Error )
{
    // NULL checks
    if( Mutex == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "Mutex is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    if( 0 != pthread_mutex_destroy( Mutex->pthread_mutex ) )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "pthread_mutex_destroy failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    return fphig_free( &Mutex->pthread_mutex,
                       Error );
}

#endif
