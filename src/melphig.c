#include "melphig/melphig.h"

#include <string.h>
#include <stdio.h>

fphig
fphig_error_message( enum fphig_error_type          Error_Type,
                     const char*                    Message,
                     struct fphig_error*            Error,
                     FPHIG_OPTIONAL const char*   File,
                     FPHIG_OPTIONAL const char*   Function,
                     FPHIG_OPTIONAL size_t        Line )
{
    // The return fail can never really be caught but prevent further damage
    if( Message == NULL || Error == NULL )
        return FPHIG_FAIL;

    Error->error_type       = Error_Type;
    Error->message_size     = FPHIG_ERROR_MESSAGE_SIZE;
    Error->file_size        = FPHIG_ERROR_FILE_SIZE;
    Error->function_size    = FPHIG_ERROR_FUNCTION_SIZE;
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

    return FPHIG_OK;
}


