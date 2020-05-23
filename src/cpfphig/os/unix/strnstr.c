#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_STRNSTR

#include "cpfphig/strnstr.h"

#include <string.h>


cpfphig
cpfphig_strnstr( const char*                            Haystack,
                 const char*                            Needle,
                 size_t                                 Len,
                 char** const                           Occurence,
                 CPFPHIG_OPTIONAL struct cpfphig_error* Error )
{
    // NULL checks
    if( Haystack == NULL || Needle == NULL || Occurence == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message( cpfphig_system_error, "Haystack, Needle or Occurence is NULL", Error );

        return CPFPHIG_FAIL;
    }

    if( 0 == strnlen( Needle, Len ) )
    {
        *Occurence = NULL;
        return CPFPHIG_OK;
    }

    *Occurence = strnstr( Haystack, Needle, Len );

    return CPFPHIG_OK;
}

#endif
