#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_WINDOWS_H

#include "cpfphig/malloc.h"
#include "cpfphig/mutex_init.h"
#include "cpfphig/mutex.h"
#include "cpfphig/mutex_attr.h"
#include "cpfphig/mutex_destroy.h"

#include <windows.h>

cpfphig
cpfphig_mutex_init( struct cpfphig_mutex*                    Mutex,
                    struct cpfphig_mutex_attr*               Mutex_Attr,
                    CPFPHIG_OPTIONAL struct cpfphig_error*   Error )
{
    // NULL checks
    if( Mutex == NULL || Mutex_Attr == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Mutex or Mutex_Attr is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }


    if( CPFPHIG_OK == ( ret = cpfphig_malloc( sizeof( CRITICAL_SECTION ),
                                              &Mutex->critical_section,
                                              Error ) ) )
    {
        InitializeCriticalSection( Mutex->critical_section ); // return void
    }

    return MELPHIG_OK;
}

#endif
