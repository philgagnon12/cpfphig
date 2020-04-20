#include "melphig/melphig.h"
#include "melphig/free.h"

#include "stdlib.h"

mphig
mphig_free( void*                                   Ptr,
            MELPHIG_OPTIONAL struct mphig_error*    Error )
{
    // NULL check
    if( Ptr == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Ptr is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    free( *(void**)Ptr );
    *(void**)Ptr = NULL;

    return MELPHIG_OK;
}

