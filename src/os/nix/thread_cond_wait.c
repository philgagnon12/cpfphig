#include "melphig/melphig.h"

#ifdef MELPHIG_HAVE_PTHREAD_H

#include "melphig/melphig.h"
#include "melphig/thread_cond.h"
#include "melphig/mutex.h"

#include <pthread.h>

mphig
mphig_thread_cond_wait( struct mphig_thread_cond*               Cond,
                        struct mphig_mutex*                     Mutex,
                        MELPHIG_OPTIONAL struct mphig_error*    Error )
{
    // NULL checks
    if( Cond == NULL || Mutex == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Cond or Mutex is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    if( 0 != pthread_cond_wait( Cond->pthread_cond,
                                Mutex->pthread_mutex ) )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "pthread_cond_wait failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    return MELPHIG_OK;
}

#endif
