#include "melphig/melphig.h"

#ifdef MELPHIG_HAVE_PTHREAD_H

#include "melphig/melphig.h"
#include "melphig/thread.h"
#include "melphig/thread_create.h"

#include <pthread.h>

mphig
mphig_thread_create( struct mphig_thread*                   Thread,
                     struct mphig_thread_attr*              Thread_Attr,
                     void* (*Start_Routine)(void*),
                     MELPHIG_OPTIONAL void*                 Arg,
                     MELPHIG_OPTIONAL struct mphig_error*   Error )
{
    // NULL checks
    if( Thread == NULL || Thread_Attr == NULL || Start_Routine == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Thread, Thread_Attr or Start_Routine is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    if( 0 != pthread_attr_init( &(Thread_Attr->pthread_attr) ) )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "pthread_attr_init failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    if( 0 != pthread_create( &(Thread->pthread),
                             &(Thread_Attr->pthread_attr),
                             Start_Routine,
                             Arg ) )
    {
         if( Error != NULL )
            mphig_error_message(mphig_system_error, "pthread_create failed", Error, __FILE__, __FUNCTION__, __LINE__ );

         return MELPHIG_FAIL;
    }

    if( 0 != pthread_attr_destroy( &(Thread_Attr->pthread_attr) ) )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "pthread_attr_destroy failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    return MELPHIG_OK;
}

#endif
