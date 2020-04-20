#include "fphig/fphig.h"

#ifdef FPHIG_HAVE_PTHREAD_H

#include "fphig/thread_cond_signal.h"
#include "fphig/thread_cond.h"

#include <pthread.h>

fphig
fphig_thread_cond_signal( struct fphig_thread_cond*             Cond,
                          FPHIG_OPTIONAL struct fphig_error*  Error )

{
    // NULL checks
    if( Cond == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "Cond is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    if( 0 != pthread_cond_signal( Cond->pthread_cond ) )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "pthread_cond_signal failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    return FPHIG_OK;
}

#endif