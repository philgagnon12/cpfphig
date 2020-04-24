#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_WINDOWS_H

#include "cpfphig/thread_join.h"
#include "cpfphig/thread.h"

#include <windows.h>

cpfphig
cpfphig_thread_join( struct cpfphig_thread*                 Thread,
                     CPFPHIG_OPTIONAL int*                  Thread_Ret,
                     CPFPHIG_OPTIONAL struct cpfphig_error* Error )
{
    DWORD thread_ret = 0;

    // NULL checks
    if( Thread == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Thread is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    if( 0 != WaitForSingleObject( Thread->handle,
                                  INFINITE ) )
    {
        return CPFPHIG_FAIL;
    }

    if( 0 == GetExitCodeThread( Thread->handle,
                                &thread_ret ) )
    {
        return CPFPHIG_FAIL;
    }

    if( Thread_Ret != NULL )
        *Thread_Ret = (int)thread_ret;

    return CPFPHIG_OK
}

#endif
