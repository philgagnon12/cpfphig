#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_DLFCN_H

#include "cpfphig/cpfphig.h"
#include "cpfphig/library_load.h"

#include <dlfcn.h>
#include <string.h>
#include <stdio.h>

cpfphig
cpfphig_library_load( const char*                             File_Name,
                      void**                                  Handle,
                      CPFPHIG_OPTIONAL struct cpfphig_error*    Error )
{
    void*   handle  = NULL;

    // NULL checks
    if( File_Name == NULL || Handle == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message( cpfphig_system_error, "File_Name or Handle is NULL", Error );

        return CPFPHIG_FAIL;
    }

    if( NULL == ( handle = dlopen( File_Name, RTLD_NOW ) ) )
    {
        if( Error != NULL )
        {
            cpfphig_error_message( cpfphig_system_error, "dlopen failed on %s", Error, File_Name );
        }

        return CPFPHIG_FAIL;
    }

    *Handle = handle;

    return CPFPHIG_OK;
}

#endif

