#include "melphig/melphig.h"

#ifdef MELPHIG_HAVE_DLFCN_H

#include "melphig/melphig.h"
#include "melphig/library_load.h"

#include <dlfcn.h>
#include <string.h>
#include <stdio.h>

#define MELPHIG_BUFFER_SIZE ( 0x0AFF )

mphig
mphig_library_load( const char*                             File_Name,
                    void**                                  Handle,
                    MELPHIG_OPTIONAL struct mphig_error*    Error )
{
    void*   handle  = NULL;
    char    error_message_buffer[MELPHIG_BUFFER_SIZE];

    // NULL checks
    if( File_Name == NULL || Handle == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "File_Name or Handle is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    if( NULL == ( handle = dlopen( File_Name, RTLD_NOW ) ) )
    {
        if( Error != NULL )
            {
                // Reset
                memset( error_message_buffer,
                        0x00,
                        MELPHIG_BUFFER_SIZE );

                snprintf( error_message_buffer,
                          MELPHIG_BUFFER_SIZE,
                          "dlopen failed on %s",
                          File_Name );

                mphig_error_message(mphig_system_error, error_message_buffer, Error, __FILE__, __FUNCTION__, __LINE__ );
            }

        return MELPHIG_FAIL;
    }

    *Handle = handle;

    return MELPHIG_OK;
}

#endif

