#include "melphig/melphig.h"
#include "melphig/malloc.h"

#include <stdlib.h>

mphig
mphig_malloc( const size_t                          Size,
              void*                                 Ptr,
              MELPHIG_OPTIONAL struct mphig_error*  Error )
{
    // NULL checks
    if( Ptr == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Ptr is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    *(void**)Ptr = malloc( Size );

    if( *(void**)Ptr == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "malloc did not allocate", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    return MELPHIG_OK;
}


