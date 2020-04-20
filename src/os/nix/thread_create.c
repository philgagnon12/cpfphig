#include "fphig/fphig.h"

#ifdef FPHIG_HAVE_PTHREAD_H

#include "fphig/fphig.h"
#include "fphig/thread.h"
#include "fphig/thread_create.h"

#include <pthread.h>

fphig
fphig_thread_create( struct fphig_thread*                   Thread,
                     struct fphig_thread_attr*              Thread_Attr,
                     void* (*Start_Routine)(void*),
                     FPHIG_OPTIONAL void*                 Arg,
                     FPHIG_OPTIONAL struct fphig_error*   Error )
{
    // NULL checks
    if( Thread == NULL || Thread_Attr == NULL || Start_Routine == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "Thread, Thread_Attr or Start_Routine is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    if( 0 != pthread_attr_init( &(Thread_Attr->pthread_attr) ) )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "pthread_attr_init failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    if( 0 != pthread_create( &(Thread->pthread),
                             &(Thread_Attr->pthread_attr),
                             Start_Routine,
                             Arg ) )
    {
         if( Error != NULL )
            fphig_error_message(fphig_system_error, "pthread_create failed", Error, __FILE__, __FUNCTION__, __LINE__ );

         return FPHIG_FAIL;
    }

    if( 0 != pthread_attr_destroy( &(Thread_Attr->pthread_attr) ) )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "pthread_attr_destroy failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    return FPHIG_OK;
}

#endif
