#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_WINDOWS_H

#include "cpfphig/thread_cond_destroy.h"
#include "cpfphig/thread_cond.h"
#include "cpfphig/free.h"

#include <windows.h>

cpfphig
cpfphig_thread_cond_destroy( struct cpfphig_thread_cond*               Cond,
                             CPFPHIG_OPTIONAL struct cpfphig_error*    Error )
{
    // NULL checks
    if( Cond == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Cond is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    return cpfphig_free( &Cond->condition_variable,
                         NULL );
}

#endif
