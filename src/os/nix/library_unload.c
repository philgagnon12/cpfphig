#include "melphig/melphig.h"

#ifdef MELPHIG_HAVE_DLFCN_H

#include "melphig/melphig.h"
#include "melphig/library_unload.h"

#include <dlfcn.h>

mphig
mphig_library_unload( void*                                    Handle,
                      MELPHIG_OPTIONAL struct mphig_error*     Error )
{
    // NULL checks
    if( Handle == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Handle is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    if( 0 != dlclose( Handle ) )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "dlclose failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    return MELPHIG_OK;
}

#endif

