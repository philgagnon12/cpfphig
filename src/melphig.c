#include "melphig/melphig.h"

#include <string.h>
#include <stdio.h>

mphig
mphig_error_message( enum mphig_error_type          Error_Type,
                     const char*                    Message,
                     struct mphig_error*            Error,
                     MELPHIG_OPTIONAL const char*   File,
                     MELPHIG_OPTIONAL const char*   Function,
                     MELPHIG_OPTIONAL size_t        Line )
{
    // The return fail can never really be caught but prevent further damage
    if( Message == NULL || Error == NULL )
        return MELPHIG_FAIL;

    Error->error_type       = Error_Type;
    Error->message_size     = MELPHIG_ERROR_MESSAGE_SIZE;
    Error->file_size        = MELPHIG_ERROR_FILE_SIZE;
    Error->function_size    = MELPHIG_ERROR_FUNCTION_SIZE;
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

    return MELPHIG_OK;
}


