#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_PTHREAD_H

#include "cpfphig/cpfphig.h"
#include "cpfphig/thread.h"
#include "cpfphig/thread_create.h"

#include <pthread.h>

cpfphig
cpfphig_thread_create( struct cpfphig_thread*                   Thread,
                       struct cpfphig_thread_attr*              Thread_Attr,
                       int (*Start_Routine)(void*),
                       CPFPHIG_OPTIONAL void*                   Arg,
                       CPFPHIG_OPTIONAL struct cpfphig_error*   Error )
{
    // NULL checks
    if( Thread == NULL || Thread_Attr == NULL || Start_Routine == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Thread, Thread_Attr or Start_Routine is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    if( 0 != pthread_attr_init( &(Thread_Attr->pthread_attr) ) )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "pthread_attr_init failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    if( 0 != pthread_create( &(Thread->pthread),
                             &(Thread_Attr->pthread_attr),
                             Start_Routine,
                             Arg ) )
    {
         if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "pthread_create failed", Error, __FILE__, __FUNCTION__, __LINE__ );

         return CPFPHIG_FAIL;
    }

    if( 0 != pthread_attr_destroy( &(Thread_Attr->pthread_attr) ) )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "pthread_attr_destroy failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    return CPFPHIG_OK;
}

#endif
