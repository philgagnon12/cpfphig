#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_WINDOWS_H

#include "cpfphig/cpfphig.h"
#include "cpfphig/library_unload.h"

#include <windows.h>


cpfphig
cpfphig_library_unload( void*                                   Handle,
                        CPFPHIG_OPTIONAL struct cpfphig_error*  Error )
{
    // NULL checks
    if( Handle == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Handle is NULL", Error );

        return CPFPHIG_FAIL;
    }

    if( 0 == FreeLibrary( (HMODULE)Handle ) )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "FreeLibrary failed", Error );

        return CPFPHIG_FAIL;
    }

    return CPFPHIG_OK;
}

#endif

