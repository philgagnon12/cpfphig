#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_WINDOWS_H

#include "cpfphig/thread_cond_init.h"
#include "cpfphig/malloc.h"
#include "cpfphig/thread_cond.h"
#include "cpfphig/thread_cond_attr.h"

#include <stdio.h>

#include <windows.h>

cpfphig
cpfphig_thread_cond_init( struct cpfphig_thread_cond*               Cond,
                          struct cpfphig_thread_cond_attr*          Cond_Attr,
                          CPFPHIG_OPTIONAL struct cpfphig_error*    Error )
{
    cpfphig               ret                             =   CPFPHIG_FAIL;

    // NULL checks
    if( Cond == NULL || Cond_Attr == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message( cpfphig_system_error, "Cond or Cond_Attr is NULL", Error );

        return CPFPHIG_FAIL;
    }

    if( CPFPHIG_OK == ( ret = cpfphig_malloc( sizeof( CONDITION_VARIABLE ),
                                              &Cond->condition_variable,
                                              Error ) ) )
    {
        InitializeConditionVariable( Cond->condition_variable );
    }

    return ret;
}

#endif
