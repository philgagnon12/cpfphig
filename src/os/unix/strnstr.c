#include "melphig/melphig.h"

#ifdef FPHIG_HAVE_STRNSTR

#include "melphig/strnstr.h"

#include <string.h>


fphig
fphig_strnstr( const char*                          Haystack,
               const char*                          Needle,
               size_t                               Len,
               char** const                         Occurence,
               FPHIG_OPTIONAL struct fphig_error* Error )
{
    // NULL checks
    if( Haystack == NULL || Needle == NULL || Occurence == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "Haystack, Needle or Occurence is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    if( 0 == strnlen( Needle, Len ) )
    {
        *Occurence = NULL;
        return FPHIG_OK;
    }

    *Occurence = strnstr( Haystack, Needle, Len );

    return FPHIG_OK;
}

#endif
