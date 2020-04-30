#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_WINDOWS_H

#include "cpfphig/cpfphig.h"
#include "cpfphig/library_load.h"

#include <windows.h>
#include <Libloaderapi.h>

#include <string.h>
#include <stdio.h>

#define CPFPHIG_BUFFER_SIZE ( 0x0AFF )

cpfphig
cpfphig_library_load( const char*                               File_Name,
                      void**                                    Handle,
                      CPFPHIG_OPTIONAL struct cpfphig_error*    Error )
{
    HMODULE handle  = NULL;
    char    error_message_buffer[CPFPHIG_BUFFER_SIZE];

    // NULL checks
    if( File_Name == NULL || Handle == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "File_Name or Handle is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    if( NULL == ( handle = LoadLibraryA( File_Name ) ) )
    {
        if( Error != NULL )
            {
                // Reset
                memset( error_message_buffer,
                        0x00,
                        CPFPHIG_BUFFER_SIZE );

                snprintf( error_message_buffer,
                          CPFPHIG_BUFFER_SIZE,
                          "LoadLibraryA failed on %s",
                          File_Name );

                cpfphig_error_message(cpfphig_system_error, error_message_buffer, Error, __FILE__, __FUNCTION__, __LINE__ );
            }

        return CPFPHIG_FAIL;
    }

    *Handle = (void*)handle;

    return CPFPHIG_OK;
}

#endif

