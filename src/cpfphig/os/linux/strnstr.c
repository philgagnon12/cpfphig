#include "cpfphig/cpfphig.h"

#ifndef CPFPHIG_HAVE_STRNSTR

#include "cpfphig/strnstr.h"
#include "cpfphig/malloc.h"
#include "cpfphig/free.h"


#include <string.h>
#include <stdio.h>

cpfphig
cpfphig_strnstr( const char*                          Haystack,
               const char*                          Needle,
               size_t                               Len,
               char** const                         Occurence,
               CPFPHIG_OPTIONAL struct cpfphig_error* Error )
{
    // char* haystack_buffer[Len+1];
    char* haystack_buffer = NULL;
    const char* occurence = NULL;

    // NULL checks
    if( Haystack == NULL || Needle == NULL || Occurence == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Haystack, Needle or Occurence is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    *Occurence = NULL;

    if( 0 == strnlen( Needle, Len ) )
    {
        *Occurence = NULL;
        return CPFPHIG_OK;
    }

    if( CPFPHIG_FAIL == cpfphig_malloc( Len+1,
                                        &haystack_buffer,
                                        Error ) )
    {
        return CPFPHIG_FAIL;
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

    return cpfphig_free( &haystack_buffer,
                         NULL );
}

#endif
