#include "melphig/melphig.h"

#ifdef FPHIG_HAVE_PTHREAD_H

#include "melphig/thread_join.h"
#include "melphig/thread.h"

#include <pthread.h>

fphig
fphig_thread_join( struct fphig_thread*                 Thread,
                   FPHIG_OPTIONAL void**              Value_Ptr,
                   FPHIG_OPTIONAL struct fphig_error* Error )
{
    // NULL checks
    if( Thread == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "Thread is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    if( 0 != pthread_join( Thread->pthread,
                           Value_Ptr ) )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "pthread_join failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    return FPHIG_OK;
}

#endif
