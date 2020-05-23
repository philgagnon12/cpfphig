#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_WINDOWS_H
#include "cpfphig/cpfphig.h"
#include "cpfphig/library_load.h"

#include <windows.h>

#include <string.h>
#include <stdio.h>


cpfphig
cpfphig_library_load( const char*                               File_Name,
                      void**                                    Handle,
                      CPFPHIG_OPTIONAL struct cpfphig_error*    Error )
{
    HMODULE handle  = NULL;

    // NULL checks
    if( File_Name == NULL || Handle == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message( cpfphig_system_error, "File_Name or Handle is NULL", Error );

        return CPFPHIG_FAIL;
    }

    if( NULL == ( handle = LoadLibraryA( File_Name ) ) )
    {
        if( Error != NULL )
        {
            cpfphig_error_message(cpfphig_system_error, "LoadLibraryA failed on %s", Error, File_Name );
        }

        return CPFPHIG_FAIL;
    }

    *Handle = (void*)handle;

    return CPFPHIG_OK;
}

#endif

