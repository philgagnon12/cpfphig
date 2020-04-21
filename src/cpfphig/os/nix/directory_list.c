#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_DIRENT_H

#include "cpfphig/directory_list.h"
#include "cpfphig/destroy_directory_list.h"
#include "cpfphig/list.h"
#include "cpfphig/malloc.h"
#include "cpfphig/free.h"

#include <dirent.h>
#include <errno.h>
#include <string.h>

#define CPFPHIG_BUFFER_SIZE ( 0x04FF )

cpfphig
REAL(cpfphig_directory_list)( const char*                           Directory,
                            struct cpfphig_list*                    File_Names,
                            CPFPHIG_OPTIONAL struct cpfphig_error*  Error )
{
    cpfphig           ret         = CPFPHIG_FAIL;
    DIR*            dir         = NULL;
    struct dirent*  dirent      = NULL;
    char*           file_name   = NULL;
    size_t          d_name_len  = 0;

    // NULL checks
    if( Directory == NULL || File_Names == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Directory or File_Names is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    if( NULL == ( dir = opendir( Directory ) ) )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "opendir failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    // Assume ok
    errno = 0;
    ret = CPFPHIG_OK;

    while( NULL != ( dirent = readdir( dir ) ) && errno == 0 && ret == CPFPHIG_OK )
    {
        file_name = NULL;
        if( dirent->d_type == DT_REG )
        {
            d_name_len = strnlen( dirent->d_name, CPFPHIG_BUFFER_SIZE-1 );

            if( CPFPHIG_FAIL == ( ret = cpfphig_malloc( d_name_len+1,
                                                      &file_name,
                                                      Error ) ) )
            {
                break;
            }

            memcpy( file_name,
                    dirent->d_name,
                    d_name_len );

            file_name[d_name_len] = 0x00;

            if( CPFPHIG_FAIL == ( ret = cpfphig_list_push( File_Names,
                                                         file_name,
                                                         Error ) ) )
            {
                break;
            }
        }
    } // while

    if( dirent == NULL && errno != 0 )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "readdir failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    if( 0 != closedir( dir ) )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "closedir failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    if( ret != CPFPHIG_OK )
    {
        cpfphig_free( &file_name, NULL );
        cpfphig_destroy_directory_list( File_Names, NULL );
    }

    return ret;
}

#endif
