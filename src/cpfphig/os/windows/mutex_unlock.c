#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_WINDOWS_H

#include "cpfphig/mutex_unlock.h"
#include "cpfphig/mutex.h"

#include <windows.h>

cpfphig
cpfphig_mutex_unlock( struct cpfphig_mutex*                    Mutex,
                      CPFPHIG_OPTIONAL struct cpfphig_error*   Error )
{
    // NULL checks
    if( Mutex == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Mutex is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    LeaveCriticalSection( Mutex->critical_section ); // return void

    return CPFPHIG_OK;
}

#endif
