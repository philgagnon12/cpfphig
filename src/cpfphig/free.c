#include "cpfphig/cpfphig.h"
#include "cpfphig/free.h"

#include "stdlib.h"

cpfphig
cpfphig_free( void*                                   Ptr,
            CPFPHIG_OPTIONAL struct cpfphig_error*    Error )
{
    // NULL check
    if( Ptr == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Ptr is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    free( *(void**)Ptr );
    *(void**)Ptr = NULL;

    return CPFPHIG_OK;
}

