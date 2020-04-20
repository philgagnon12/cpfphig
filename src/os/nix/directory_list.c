#include "melphig/melphig.h"

#ifdef MELPHIG_HAVE_DIRENT_H

#include "melphig/directory_list.h"
#include "melphig/destroy_directory_list.h"
#include "melphig/list.h"
#include "melphig/malloc.h"
#include "melphig/free.h"

#include <dirent.h>
#include <errno.h>
#include <string.h>

#define MELPHIG_BUFFER_SIZE ( 0x04FF )

mphig
REAL(mphig_directory_list)( const char*                           Directory,
                            struct mphig_list*                    File_Names,
                            MELPHIG_OPTIONAL struct mphig_error*  Error )
{
    mphig           ret         = MELPHIG_FAIL;
    DIR*            dir         = NULL;
    struct dirent*  dirent      = NULL;
    char*           file_name   = NULL;
    size_t          d_name_len  = 0;

    // NULL checks
    if( Directory == NULL || File_Names == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Directory or File_Names is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    if( NULL == ( dir = opendir( Directory ) ) )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "opendir failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    // Assume ok
    errno = 0;
    ret = MELPHIG_OK;

    while( NULL != ( dirent = readdir( dir ) ) && errno == 0 && ret == MELPHIG_OK )
    {
        file_name = NULL;
        if( dirent->d_type == DT_REG )
        {
            d_name_len = strnlen( dirent->d_name, MELPHIG_BUFFER_SIZE-1 );

            if( MELPHIG_FAIL == ( ret = mphig_malloc( d_name_len+1,
                                                      &file_name,
                                                      Error ) ) )
            {
                break;
            }

            memcpy( file_name,
                    dirent->d_name,
                    d_name_len );

            file_name[d_name_len] = 0x00;

            if( MELPHIG_FAIL == ( ret = mphig_list_push( File_Names,
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
            mphig_error_message(mphig_system_error, "readdir failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    if( 0 != closedir( dir ) )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "closedir failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    if( ret != MELPHIG_OK )
    {
        mphig_free( &file_name, NULL );
        mphig_destroy_directory_list( File_Names, NULL );
    }

    return ret;
}

#endif
