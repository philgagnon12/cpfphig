#include "cpfphig/cpfphig.h"
#include "cpfphig/error_message_fprintf.h"
#include "cpfphig/error_message_allocated_message.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

cpfphig
cpfphig_error_message_call( enum cpfphig_error_type          Error_Type,
                            const char*                      Format,
                            CPFPHIG_OPTIONAL const char*     File,
                            CPFPHIG_OPTIONAL const char*     Function,
                            CPFPHIG_OPTIONAL int             Line,
                            struct cpfphig_error*            Error,
                            ... )
{
    cpfphig ret = CPFPHIG_FAIL;
    va_list args;

    // The return fail can never really be caught but prevent further damage
    if( Format == NULL || Error == NULL )
        return CPFPHIG_FAIL;

    Error->error_type   = Error_Type;
    Error->file         = File;         // REMARK expected to be __FILE__
    Error->function     = Function;     // REMARK expected to be __FUNCTION__
    Error->line         = Line;

    va_start( args, Error );
    switch( Error->error_component_type )
    {
        case cpfphig_error_fprintf:
            ret = cpfphig_error_message_fprintf( Error,
                                                 Format,
                                                 args );
            break;
        case cpfphig_error_allocated_message:
            ret = cpfphig_error_message_allocated_message( Error,
                                                           Format,
                                                           args );
            break;
        default:
            ret = CPFPHIG_FAIL;
    }
    va_end( args );

    return ret;
}


