#include "cpfphig/cpfphig.h"
#include "cpfphig/error_message_fprintf.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

cpfphig
cpfphig_error_message_fprintf( struct cpfphig_error*            Error,
                               const char*                      Format,
                               va_list                          Args )
{
    // The return fail can never really be caught but prevent further damage
    if( Error == NULL || Format == NULL )
        return CPFPHIG_FAIL;

    if( Error->error_component_type != cpfphig_error_fprintf )
        return CPFPHIG_FAIL;

    if( Error->error_component.fprintf.file == NULL )
        return CPFPHIG_FAIL;

    Error->error_component.fprintf.file_pos = ftell( Error->error_component.fprintf.file );

    Error->error_component.fprintf.log_len = 0;

    if( Error->file != NULL )
    {
        Error->error_component.fprintf.log_len += fprintf( Error->error_component.fprintf.file,
                                                           "%s",
                                                           Error->file );
    }
    if( Error->function != NULL )
    {
        Error->error_component.fprintf.log_len += fprintf( Error->error_component.fprintf.file,
                                                           "(%s):",
                                                           Error->function );
    }

    Error->error_component.fprintf.log_len += fprintf( Error->error_component.fprintf.file,
                                                       "%d; ",
                                                       Error->line );

    Error->error_component.fprintf.log_len += vfprintf( Error->error_component.fprintf.file, Format, Args );

    Error->error_component.fprintf.log_len += fprintf( Error->error_component.fprintf.file, "\n" );

    return CPFPHIG_OK;
}


