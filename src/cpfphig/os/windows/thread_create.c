#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_WINDOWS_H

#include "cpfphig/cpfphig.h"
#include "cpfphig/thread.h"
#include "cpfphig/thread_create.h"

#include <windows.h>

cpfphig
cpfphig_thread_create( struct cpfphig_thread*                   Thread,
                       struct cpfphig_thread_attr*              Thread_Attr,
                       void* (*Start_Routine)(void*),
                       CPFPHIG_OPTIONAL void*                   Arg,
                       CPFPHIG_OPTIONAL struct cpfphig_error*   Error )
{
    HANDLE handle = NULL;

    // NULL checks
    if( Thread == NULL || Thread_Attr == NULL || Start_Routine == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Thread, Thread_Attr or Start_Routine is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    handle = CreateThread( Thread_Attr->lp_thread_attributes,
                           Thread_Attr->dw_stack_size,
                           ( LPTHREAD_START_ROUTINE )Start_Routine,
                           Arg,
                           Thread_Attr->dw_creation_flags,
                           NULL );

    if( handle == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "CreateThread failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    Thread->handle = handle;

    return CPFPHIG_OK;
}

#endif
