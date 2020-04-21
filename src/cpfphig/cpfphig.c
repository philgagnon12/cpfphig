#include "cpfphig/cpfphig.h"

#include <string.h>
#include <stdio.h>

cpfphig
cpfphig_error_message( enum cpfphig_error_type          Error_Type,
                     const char*                    Message,
                     struct cpfphig_error*            Error,
                     CPFPHIG_OPTIONAL const char*   File,
                     CPFPHIG_OPTIONAL const char*   Function,
                     CPFPHIG_OPTIONAL size_t        Line )
{
    // The return fail can never really be caught but prevent further damage
    if( Message == NULL || Error == NULL )
        return CPFPHIG_FAIL;

    Error->error_type       = Error_Type;
    Error->message_size     = CPFPHIG_ERROR_MESSAGE_SIZE;
    Error->file_size        = CPFPHIG_ERROR_FILE_SIZE;
    Error->function_size    = CPFPHIG_ERROR_FUNCTION_SIZE;
    Error->line             = 0;

    memset( Error->message,
            0x00,
            Error->message_size );

    snprintf( Error->message,
              Error->message_size,
              "%s",
              Message );


    if( File != NULL )
    {
        memset( Error->file,
                0x00,
                Error->file_size );

        snprintf( Error->file,
                  Error->file_size,
                  "%s",
                  File );
    }

    if( Function != NULL )
    {
        memset( Error->function,
                0x00,
                Error->function_size );

        snprintf( Error->function,
                  Error->function_size,
                  "%s",
                  Function );
    }


    if( Line != 0 )
    {
        Error->line = Line;
    }

    return CPFPHIG_OK;
}


