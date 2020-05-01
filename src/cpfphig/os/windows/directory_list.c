#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_WINDOWS_H

#include "cpfphig/directory_list.h"
#include "cpfphig/destroy_directory_list.h"
#include "cpfphig/list.h"
#include "cpfphig/malloc.h"
#include "cpfphig/free.h"

#include <windows.h>
#include <string.h>
#include <stdio.h>

#define CPFPHIG_BUFFER_SIZE ( 0x04FF )

cpfphig
CPFPHIG_REAL(cpfphig_directory_list)( const char*                             Directory,
                              struct cpfphig_list*                    File_Names,
                              CPFPHIG_OPTIONAL struct cpfphig_error*  Error )
{
    cpfphig           ret         = CPFPHIG_FAIL;
    char              directory[CPFPHIG_BUFFER_SIZE];
    HANDLE            dir         = NULL;
    WIN32_FIND_DATA   find_data   = { 0 };
    BOOL              found_data  = 1;
    char*             file_name   = NULL;
    size_t            d_name_len  = 0;

    // NULL checks
    if( Directory == NULL || File_Names == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Directory or File_Names is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    memset( directory,
            0x00,
            CPFPHIG_BUFFER_SIZE );

    snprintf( directory,
              CPFPHIG_BUFFER_SIZE,
              "%s\\*",
              Directory );

    if( NULL == ( dir = FindFirstFile( directory, &find_data ) ) )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "FindFirstFile failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    // Assume ok
    ret = CPFPHIG_OK;

    while( ret == CPFPHIG_OK && found_data == 1 )
    {
        file_name = NULL;

        d_name_len = strnlen( find_data.cFileName, CPFPHIG_BUFFER_SIZE-1 );

        if( CPFPHIG_FAIL == ( ret = cpfphig_malloc( d_name_len+1,
                                                    &file_name,
                                                    Error ) ) )
        {
            break;
        }

        memcpy( file_name,
                find_data.cFileName,
                d_name_len );

        file_name[d_name_len] = 0x00;

        if( CPFPHIG_FAIL == ( ret = cpfphig_list_push( File_Names,
                                                       file_name,
                                                       Error ) ) )
        {
            break;
        }

        found_data = FindNextFile( dir, &find_data );
    } // while

    if( found_data == 0 && ERROR_NO_MORE_FILES != GetLastError() )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "FindNextFile failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        ret = CPFPHIG_FAIL;
    }

    if( 0 == FindClose( dir ) && ret == CPFPHIG_OK )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "FindClose failed", Error, __FILE__, __FUNCTION__, __LINE__ );

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
