#include "fphig/fphig.h"
#include "fphig/malloc.h"

#include <stdlib.h>

fphig
fphig_malloc( const size_t                          Size,
              void*                                 Ptr,
              FPHIG_OPTIONAL struct fphig_error*  Error )
{
    // NULL checks
    if( Ptr == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "Ptr is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    *(void**)Ptr = malloc( Size );

    if( *(void**)Ptr == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "malloc did not allocate", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    return FPHIG_OK;
}


