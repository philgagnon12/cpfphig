#include "melphig/melphig.h"

#ifdef MELPHIG_HAVE_PTHREAD_H

#include "melphig/thread_cond_broadcast.h"
#include "melphig/thread_cond.h"

#include <pthread.h>

mphig
mphig_thread_cond_broadcast( struct mphig_thread_cond*             Cond,
                             MELPHIG_OPTIONAL struct mphig_error*  Error )
{
    // NULL checks
    if( Cond == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Cond is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    if( 0 != pthread_cond_broadcast( Cond->pthread_cond ) )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "pthread_cond_broadcast failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    return MELPHIG_OK;
}

#endif
