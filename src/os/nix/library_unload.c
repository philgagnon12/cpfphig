#include "melphig/melphig.h"

#ifdef FPHIG_HAVE_DLFCN_H

#include "melphig/melphig.h"
#include "melphig/library_unload.h"

#include <dlfcn.h>

fphig
fphig_library_unload( void*                                    Handle,
                      FPHIG_OPTIONAL struct fphig_error*     Error )
{
    // NULL checks
    if( Handle == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "Handle is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    if( 0 != dlclose( Handle ) )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "dlclose failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    return FPHIG_OK;
}

#endif

