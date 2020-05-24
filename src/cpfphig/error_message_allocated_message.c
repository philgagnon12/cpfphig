#include "cpfphig/cpfphig.h"
#include "cpfphig/error_message_allocated_message.h"

#include "cpfphig/malloc.h"
#include "cpfphig/free.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#define CPFPHIG_BUFFER_SIZE ( 0xFFFF )

cpfphig
cpfphig_error_message_allocated_message( struct cpfphig_error*            Error,
                                         const char*                      Format,
                                         va_list                          Args )
{
    cpfphig ret             = CPFPHIG_FAIL;
    va_list args_copy;

    char    message_predict_buffer[sizeof(char)];
    char*   message         = NULL;
    int     message_len     = 0;

    // The return fail can never really be caught but prevent further damage
    if( Error == NULL || Format == NULL )
        return CPFPHIG_FAIL;

    if( Error->error_component_type != cpfphig_error_allocated_message )
        return CPFPHIG_FAIL;

    va_copy( args_copy, Args );

    message_len = vsnprintf( message_predict_buffer,
                             sizeof( char ),
                             Format,
                             Args );

    ret = cpfphig_malloc( message_len + sizeof( char ),
                          &message,
                          NULL );

    if( ret == CPFPHIG_OK )
    {
        if( 0 > vsnprintf( message,
                           CPFPHIG_BUFFER_SIZE,
                           Format,
                           args_copy ) )
        {
            ret = CPFPHIG_FAIL;
        }
    }

    va_end( args_copy );

    if( ret == CPFPHIG_OK )
    {
        message[ message_len ] = 0x00;
        Error->error_component.allocated_message.message        = message;
        Error->error_component.allocated_message.message_len    = message_len;
    }

    if( ret == CPFPHIG_FAIL )
    {
        cpfphig_free( &message, NULL );
    }

    return ret;
}


