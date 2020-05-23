#include "cpfphig/cpfphig.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

cpfphig
cpfphig_stderr_printf( enum cpfphig_error_type          Error_Type,
                       const char*                      Format,
                       CPFPHIG_OPTIONAL const char*     File,
                       CPFPHIG_OPTIONAL const char*     Function,
                       CPFPHIG_OPTIONAL int             Line,
                       struct cpfphig_error*            Error,
                       ... )
{
    va_list args;

    // The return fail can never really be caught but prevent further damage
    if( Format == NULL || Error == NULL )
        return CPFPHIG_FAIL;

    Error->error_type       = Error_Type;
    Error->stderr_pos       = ftell( stderr );

    Error->log_len = fprintf( stderr, "%s(%s):%d; ", File, Function, Line );

    va_start( args, Error );
    Error->log_len += vfprintf( stderr, Format, args );
    va_end( args );

    Error->log_len += fprintf( stderr, "\n" );

    return CPFPHIG_OK;
}


