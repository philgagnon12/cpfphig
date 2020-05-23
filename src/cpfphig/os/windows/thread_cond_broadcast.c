#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_WINDOWS_H

#include "cpfphig/thread_cond_broadcast.h"
#include "cpfphig/thread_cond.h"

#include <windows.h>

cpfphig
cpfphig_thread_cond_broadcast( struct cpfphig_thread_cond*             Cond,
                               CPFPHIG_OPTIONAL struct cpfphig_error*  Error )
{
    // NULL checks
    if( Cond == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message( cpfphig_system_error, "Cond is NULL", Error );

        return CPFPHIG_FAIL;
    }

    WakeAllConditionVariable( Cond->condition_variable ); // return void

    return CPFPHIG_OK;
}

#endif
