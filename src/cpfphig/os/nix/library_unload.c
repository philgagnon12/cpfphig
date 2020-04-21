#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_DLFCN_H

#include "cpfphig/cpfphig.h"
#include "cpfphig/library_unload.h"

#include <dlfcn.h>

cpfphig
cpfphig_library_unload( void*                                    Handle,
                      CPFPHIG_OPTIONAL struct cpfphig_error*     Error )
{
    // NULL checks
    if( Handle == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Handle is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    if( 0 != dlclose( Handle ) )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "dlclose failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    return CPFPHIG_OK;
}

#endif

