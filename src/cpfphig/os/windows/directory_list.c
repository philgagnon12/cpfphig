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

#define CPFPHIG_BUFFER_SIZE ( 0xFFFF )

cpfphig
CPFPHIG_REAL(cpfphig_directory_list)( const char*                             Directory,
                                      struct cpfphig_list*                    File_Names,
                                      CPFPHIG_OPTIONAL struct cpfphig_error*  Error )
{
    cpfphig                 ret                 = CPFPHIG_FAIL;
    char*                   directory           = NULL;
    int                     directory_len       = NULL;
    HANDLE                  dir                 = NULL;
    WIN32_FIND_DATA         find_data           = { 0 };
    BOOL                    found_data          = 1;
    char*                   file_name           = NULL;
    size_t                  d_name_len          = 0;
    struct cpfphig_error    free_error          = CPFPHIG_CONST_CPFPHIG_ERROR;

    // NULL checks
    if( Directory == NULL || File_Names == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message( cpfphig_system_error, "Directory or File_Names is NULL", Error );

        return CPFPHIG_FAIL;
    }

    directory_len = strnlen( Directory, CPFPHIG_BUFFER_SIZE - sizeof( char ) );

    directory_len += sizeof("\\*") - sizeof( char ); // remove null char

    if( CPFPHIG_FAIL == ret = mphig_malloc( &directory,
                              directory_len + sizeof( char ), // add null char
                              Error ) )
    {
        return CPFPHIG_FAIL;
    }


    memset( directory,
            0x00,
            CPFPHIG_BUFFER_SIZE );

    snprintf( directory,
              directory_len + sizeof( char ),
              "%s\\*",
              Directory );

    dir = FindFirstFile( directory, &find_data );

    if( dir == NULL || dir == INVALID_HANDLE_VALUE )
    {
        if( Error != NULL )
        {
            cpfphig_error_message(cpfphig_system_error, "FindFirstFile with argument '%s' failed", Error, directory );
        }

        ret = CPFPHIG_FAIL;
    }


    if( ret == CPFPHIG_OK )
    {
        // TODO what happens when dir is not found ?
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
                cpfphig_error_message( cpfphig_system_error, "FindNextFile failed", Error );

            ret = CPFPHIG_FAIL;
        }

        if( 0 == FindClose( dir ) && ret == CPFPHIG_OK )
        {
            if( Error != NULL )
                cpfphig_error_message( cpfphig_system_error, "FindClose failed", Error );

            return CPFPHIG_FAIL;
        }
    } // dir == NULL

    // Clean up
    free_error.error_type = cpfphig_ok;


    if( CPFPHIG_FAIL == cpfphig_free( &directory,
                                      &free_error ) )
    {
        if( ret == CPFPHIG_OK )
        {
            if( Error != NULL )
                *Error = free_error;

            ret = CPFPHIG_FAIL;
        }
    }

    if( ret == CPFPHIG_FAIL )
    {
        cpfphig_free( &file_name, NULL );
        cpfphig_destroy_directory_list( File_Names, NULL );
    }

    return ret;
}

#endif
