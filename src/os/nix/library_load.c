#include "melphig/melphig.h"

#ifdef FPHIG_HAVE_DLFCN_H

#include "melphig/melphig.h"
#include "melphig/library_load.h"

#include <dlfcn.h>
#include <string.h>
#include <stdio.h>

#define FPHIG_BUFFER_SIZE ( 0x0AFF )

fphig
fphig_library_load( const char*                             File_Name,
                    void**                                  Handle,
                    FPHIG_OPTIONAL struct fphig_error*    Error )
{
    void*   handle  = NULL;
    char    error_message_buffer[FPHIG_BUFFER_SIZE];

    // NULL checks
    if( File_Name == NULL || Handle == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "File_Name or Handle is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    if( NULL == ( handle = dlopen( File_Name, RTLD_NOW ) ) )
    {
        if( Error != NULL )
            {
                // Reset
                memset( error_message_buffer,
                        0x00,
                        FPHIG_BUFFER_SIZE );

                snprintf( error_message_buffer,
                          FPHIG_BUFFER_SIZE,
                          "dlopen failed on %s",
                          File_Name );

                fphig_error_message(fphig_system_error, error_message_buffer, Error, __FILE__, __FUNCTION__, __LINE__ );
            }

        return FPHIG_FAIL;
    }

    *Handle = handle;

    return FPHIG_OK;
}

#endif

