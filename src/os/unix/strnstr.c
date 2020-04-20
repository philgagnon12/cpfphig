#include "melphig/melphig.h"

#ifdef MELPHIG_HAVE_STRNSTR

#include "melphig/strnstr.h"

#include <string.h>


mphig
mphig_strnstr( const char*                          Haystack,
               const char*                          Needle,
               size_t                               Len,
               char** const                         Occurence,
               MELPHIG_OPTIONAL struct mphig_error* Error )
{
    // NULL checks
    if( Haystack == NULL || Needle == NULL || Occurence == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Haystack, Needle or Occurence is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    if( 0 == strnlen( Needle, Len ) )
    {
        *Occurence = NULL;
        return MELPHIG_OK;
    }

    *Occurence = strnstr( Haystack, Needle, Len );

    return MELPHIG_OK;
}

#endif
