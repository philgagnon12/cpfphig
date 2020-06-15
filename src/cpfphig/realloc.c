#include "cpfphig/cpfphig.h"
#include "cpfphig/realloc.h"

#include <stdlib.h>

cpfphig
cpfphig_realloc( const size_t                               Size,
                 void*                                      Ptr_Src,
                 void**                                     Ptr_Dest,
                 CPFPHIG_OPTIONAL struct cpfphig_error*     Error )
{
    void* ptr_dest = NULL;

    // NULL checks
    if( Ptr_Src == NULL || Ptr_Dest == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Ptr_Src or Ptr_Dest is NULL", Error );

        return CPFPHIG_FAIL;
    }

     ptr_dest = realloc( Ptr_Src, Size );

    if( ptr_dest == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "realloc failed", Error );

        return CPFPHIG_FAIL;
    }

    *Ptr_Dest = ptr_dest;

    return CPFPHIG_OK;
}

