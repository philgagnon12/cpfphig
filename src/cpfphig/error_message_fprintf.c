#include "cpfphig/cpfphig.h"
#include "cpfphig/error_message_fprintf.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef CPFPHIG_HAVE_UNISTD_H
#include <unistd.h>
#else
#include <windows.h>
#include <io.h>
#define close _close
#endif

cpfphig
cpfphig_error_message_fprintf( struct cpfphig_error*            Error,
                               const char*                      Format,
                               va_list                          Args )
{
    FILE* file          = NULL;
    int   will_close    = 1;

    // The return fail can never really be caught but prevent further damage
    if( Error == NULL || Format == NULL )
        return CPFPHIG_FAIL;

    if( Error->error_component_type != cpfphig_error_fprintf )
        return CPFPHIG_FAIL;

    if( 0 != ( Error->error_component.fprintf.log_type & cpfphig_error_fprintf_log_none ) )
        return CPFPHIG_OK;

    if( 0 == ( Error->error_component.fprintf.log_type & cpfphig_error_fprintf_log_all ) )
    {
        switch( Error->error_type )
        {
            case cpfphig_system_error:
                if( 0 == (Error->error_component.fprintf.log_type & cpfphig_error_fprintf_log_type_system_error ) )
                    return CPFPHIG_OK;
                break;
            case cpfphig_user_error:
                if( 0 == (Error->error_component.fprintf.log_type & cpfphig_error_fprintf_log_type_user_error ) )
                    return CPFPHIG_OK;
                break;
            default:
                return CPFPHIG_FAIL;
        }
    }

    if( Error->error_component.fprintf.file_path == NULL )
    {
        file        = fdopen( STDERR_FILENO, "a" );
        will_close  = 0;
    }
    else
    {
        file = fopen( Error->error_component.fprintf.file_path, "a" );
    }

    if( file == NULL )
    {
        printf( "NO FILE HANDLE\n" ); // TODO remove
            return CPFPHIG_FAIL;
    }

    Error->error_component.fprintf.file_pos = ftell( file );

    Error->error_component.fprintf.log_len = 0;

    if( Error->file != NULL )
    {
        Error->error_component.fprintf.log_len += fprintf( file,
                                                           "%s",
                                                           Error->file );
    }
    if( Error->function != NULL )
    {
        Error->error_component.fprintf.log_len += fprintf( file,
                                                           "(%s):",
                                                           Error->function );
    }

    Error->error_component.fprintf.log_len += fprintf( file,
                                                       "%d; ",
                                                       Error->line );

    Error->error_component.fprintf.log_len += vfprintf( file, Format, Args );

    Error->error_component.fprintf.log_len += fprintf( file, "\n" );

    // Mainly prevent to close stderr
    if( will_close == 1 )
    {
        fclose( file );
    }

    return CPFPHIG_OK;
}


