#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_WINDOWS_H

#include "cpfphig/mutex_destroy.h"
#include "cpfphig/mutex.h"
#include "cpfphig/free.h"

#include <windows.h>

cpfphig
cpfphig_mutex_destroy( struct cpfphig_mutex*                    Mutex,
                       CPFPHIG_OPTIONAL struct cpfphig_error*   Error )
{
    // NULL checks
    if( Mutex == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Mutex is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    DeleteCriticalSection( Mutex->critical_section ); // return void

    return cpfphig_free( &Mutex->critical_section,
                         Error );
}

#endif
