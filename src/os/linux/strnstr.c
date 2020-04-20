#include "melphig/melphig.h"

#ifndef MELPHIG_HAVE_STRNSTR

#include "melphig/strnstr.h"

#include <string.h>
#include <stdio.h>


mphig
mphig_strnstr( const char*                          Haystack,
               const char*                          Needle,
               size_t                               Len,
               char** const                         Occurence,
               MELPHIG_OPTIONAL struct mphig_error* Error )
{
    char haystack_buffer[Len+1];
    const char* occurence = NULL;

    // NULL checks
    if( Haystack == NULL || Needle == NULL || Occurence == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Haystack, Needle or Occurence is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    *Occurence = NULL;

    if( 0 == strnlen( Needle, Len ) )
    {
        *Occurence = NULL;
        return MELPHIG_OK;
    }

    memset( haystack_buffer,
            0x00,
            Len+1 );

    snprintf( haystack_buffer,
              Len+1,
              "%s",
              Haystack );


    occurence = strstr( haystack_buffer, Needle );


    if( occurence != NULL )
    {
        *Occurence = (char* const)(Haystack + (occurence - haystack_buffer));
    }

    return MELPHIG_OK;
}

#endif
