#include "cpfphig/cpfphig.h"
#include "cpfphig/malloc.h"

#include <stdlib.h>

cpfphig
cpfphig_malloc( const size_t                            Size,
                void*                                   Ptr,
                CPFPHIG_OPTIONAL struct cpfphig_error*  Error )
{
    // NULL checks
    if( Ptr == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Ptr is NULL", Error );

        return CPFPHIG_FAIL;
    }

    *(void**)Ptr = malloc( Size );

    if( *(void**)Ptr == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "malloc did not allocate", Error );

        return CPFPHIG_FAIL;
    }

    return CPFPHIG_OK;
}


