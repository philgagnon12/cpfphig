#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_WINDOWS_H

#include "cpfphig/malloc.h"
#include "cpfphig/free.h"

#include <windows.h>
#include <string.h>
#include <stdio.h>

#define CPFPHIG_BUFFER_SIZE ( 0xFFFF )

cpfphig
cpfphig_dirname( const char*                               Path,
                 int                                       Path_Size,
                 char*                                     Buffer,
                 int                                       Buffer_Size,
                 CPFPHIG_OPTIONAL struct cpfphig_error*    Error )
{
    cpfphig ret     = CPFPHIG_FAIL;

    char*   drive   = NULL;
    char*   dir     = NULL;

    struct cpfphig_error    free_error  = CPFPHIG_CONST_CPFPHIG_ERROR;

    // NULL checks
    if( Path == NULL || Buffer == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Path or Buffer is NULL", Error );

        return CPFPHIG_FAIL;
    }

    ret = cpfphig_malloc( &drive,
                          Path_Size,
                          Error );

    if( ret == CPFPHIG_OK )
    {
        memset( drive,
                0x00,
                Path_Size );

        ret = cpfphig_malloc( &dir,
                              Path_size,
                              Error );
    }

    if( ret == CPFPHIG_OK )
    {
        memset( dir,
                0x00,
                Path_Size );

        if( 0 != _splitpath_s( Path,
                               drive,
                               Path_Size,
                               dir,
                               Path_Size,
                               NULL,
                               0,
                               NULL,
                               0 ) )
        {
            if( Error != NULL )
                cpfphig_error_message( cpfphig_system_error, "_splitpath_s failed", Error );

            ret = CPFPHIG_FAIL;

        }
    }

    if( ret == CPFPHIG_OK )
    {
        // Remove backslash
        dir[strnlen(dir, CPFPHIG_BUFFER_SIZE-1)-1] = 0x00;

        memset( Buffer,
                0x00,
                Buffer_Size );

        snprintf( Buffer,
                  Buffer_Size,
                  "%s%s",
                  drive,
                  dir );
    }

    free_error.error_type = cpfphig_ok;

    if( CPFPHIG_FAIL == cpfphig_free( &drive,
                                      &free_error ) )
    {
        if( ret == CPFPHIG_OK )
        {
            if( Error != NULL )
                *Error = free_error;

            ret = CPFPHIG_FAIL;
        }
    }

    if( CPFPHIG_FAIL == cpfphig_free( &dir,
                                      &free_error ) )
    {
        if( ret == CPFPHIG_OK )
        {
            if( Error != NULL )
                *Error = free_error;

            ret = CPFPHIG_FAIL;
        }
    }

    return ret;
}

#endif
