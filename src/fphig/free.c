#include "fphig/fphig.h"
#include "fphig/free.h"

#include "stdlib.h"

fphig
fphig_free( void*                                   Ptr,
            FPHIG_OPTIONAL struct fphig_error*    Error )
{
    // NULL check
    if( Ptr == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "Ptr is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    free( *(void**)Ptr );
    *(void**)Ptr = NULL;

    return FPHIG_OK;
}

