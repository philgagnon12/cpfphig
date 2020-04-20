#include "melphig/melphig.h"

#ifdef MELPHIG_HAVE_PTHREAD_H

#include "melphig/thread_join.h"
#include "melphig/thread.h"

#include <pthread.h>

mphig
mphig_thread_join( struct mphig_thread*                 Thread,
                   MELPHIG_OPTIONAL void**              Value_Ptr,
                   MELPHIG_OPTIONAL struct mphig_error* Error )
{
    // NULL checks
    if( Thread == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Thread is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    if( 0 != pthread_join( Thread->pthread,
                           Value_Ptr ) )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "pthread_join failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    return MELPHIG_OK;
}

#endif
