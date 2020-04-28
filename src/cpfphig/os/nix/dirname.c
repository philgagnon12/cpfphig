#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_LIBGEN_H

#include "cpfphig/malloc.h"
#include "cpfphig/free.h"

#include <libgen.h>
#include <string.h>

cpfphig
cpfphig_dirname( const char*                               Path,
                 int                                       Path_Size,
                 char*                                     Buffer,
                 int                                       Buffer_Size,
                 CPFPHIG_OPTIONAL struct cpfphig_error*    Error )
{
    cpfphig                 ret         = CPFPHIG_FAIL;
    char*                   path        = NULL;
    char*                   name        = NULL;
    cpfphig                 free_ret    = CPFPHIG_FAIL;
    struct cpfphig_error    free_error  = CPFPHIG_CONST_CPFPHIG_ERROR;

    // NULL checks
    if( Path == NULL || Buffer == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Path or Buffer is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    if( CPFPHIG_FAIL == ( ret = cpfphig_malloc( Path_Size,
                                                &path,
                                                Error ) ) )
    {
        return CPFPHIG_FAIL;
    }

    memset( path,
            0x00,
            Path_Size );


    memcpy( path,
            Path,
            strnlen( Path, Path_Size-1 ) );


    if( NULL == ( name = dirname( path ) ) )
    {
        if( Error != NULL )
            cpfphig_error_message( cpfphig_system_error, "dirname failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        ret = CPFPHIG_FAIL;
    }

    if( ret == CPFPHIG_OK )
    {
        memset( Buffer,
                0x00,
                Buffer_Size );

        memcpy( Buffer,
                name,
                strnlen( name, Buffer_Size-1 ) );

    }

    if( CPFPHIG_FAIL == ( free_ret = cpfphig_free( &path,
                                                   &free_error ) ) )
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
