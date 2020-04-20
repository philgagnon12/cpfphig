#include "melphig/melphig.h"

#ifdef FPHIG_HAVE_PTHREAD_H

#include "melphig/melphig.h"
#include "melphig/thread_cond.h"
#include "melphig/mutex.h"

#include <pthread.h>

fphig
fphig_thread_cond_wait( struct fphig_thread_cond*               Cond,
                        struct fphig_mutex*                     Mutex,
                        FPHIG_OPTIONAL struct fphig_error*    Error )
{
    // NULL checks
    if( Cond == NULL || Mutex == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "Cond or Mutex is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    if( 0 != pthread_cond_wait( Cond->pthread_cond,
                                Mutex->pthread_mutex ) )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "pthread_cond_wait failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    return FPHIG_OK;
}

#endif
