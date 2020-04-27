#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_PTHREAD_H

#include "cpfphig/thread_join.h"
#include "cpfphig/thread.h"

#include <pthread.h>

cpfphig
cpfphig_thread_join( struct cpfphig_thread*                 Thread,
                     CPFPHIG_OPTIONAL int*                  Thread_Ret,
                     CPFPHIG_OPTIONAL struct cpfphig_error* Error )
{
    void* value_ptr = NULL;

    // NULL checks
    if( Thread == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Thread is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    if( 0 != pthread_join( Thread->pthread,
                           &value_ptr ) )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "pthread_join failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    if( Thread_Ret != NULL )
    {
        *Thread_Ret = (int)value_ptr;
    }

    return CPFPHIG_OK;
}

#endif
