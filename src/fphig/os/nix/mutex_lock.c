#include "fphig/fphig.h"

#ifdef FPHIG_HAVE_PTHREAD_H

#include "fphig/mutex_lock.h"
#include "fphig/mutex.h"

#include <pthread.h>

fphig
fphig_mutex_lock( struct fphig_mutex*                    Mutex,
                  FPHIG_OPTIONAL struct fphig_error*   Error )
{
    // NULL checks
    if( Mutex == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "Mutex is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    if( 0 != pthread_mutex_lock( Mutex->pthread_mutex ) )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "pthread_mutex_lock failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    return FPHIG_OK;
}

#endif
