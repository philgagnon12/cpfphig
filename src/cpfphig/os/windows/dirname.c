#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_WINDOWS_H

#include <windows.h>
#include <string.h>
#include <stdio.h>

#define CPFPHIG_BUFFER_SIZE ( 0xAFF )

cpfphig
cpfphig_dirname( const char*                               Path,
                 int                                       Path_Size,
                 char*                                     Buffer,
                 int                                       Buffer_Size,
                 CPFPHIG_OPTIONAL struct cpfphig_error*    Error )
{
    cpfphig                 ret         = CPFPHIG_FAIL;

    char                    drive[CPFPHIG_BUFFER_SIZE];
    char                    dir[CPFPHIG_BUFFER_SIZE];

    // NULL checks
    if( Path == NULL || Buffer == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Path or Buffer is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    memset( drive,
            0x00,
            CPFPHIG_BUFFER_SIZE );

    memset( dir,
            0x00,
            CPFPHIG_BUFFER_SIZE );

    if( 0 != _splitpath_s( Path,
                           drive,
                           CPFPHIG_BUFFER_SIZE,
                           dir,
                           CPFPHIG_BUFFER_SIZE,
                           NULL,
                           0,
                           NULL,
                           0 ) )
    {
        if( Error != NULL )
            cpfphig_error_message( cpfphig_system_error, "_splitpath_s failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;

    }

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

    return CPFPHIG_OK;
}

#endif
