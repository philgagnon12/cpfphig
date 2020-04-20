#include "melphig/melphig.h"
#include "melphig/modules_load_all.h"
#include "melphig/directory_list.h"
#include "melphig/malloc.h"
#include "melphig/free.h"
#include "melphig/destroy_directory_list.h"
#include "melphig/strnstr.h"

#include "string.h"
#include "stdio.h"

#define MELPHIG_BUFFER_SIZE ( 0x04FF )

mphig
mphig_modules_load_all( struct mphig_list                         Modules_Directories,
                        mphig_module_load_symbol*                 Mphig_Module_Load,
                        struct mphig_list*                        Modules,
                        MELPHIG_OPTIONAL struct mphig_error*      Error )
{
    mphig                       ret                             = MELPHIG_FAIL;
    struct mphig_list_iterator  plugins_directories_iterator    = { &Modules_Directories, NULL };
    char*                       directory                       = NULL;
    mphig                       next_directory_ret              = MELPHIG_FAIL;
    struct mphig_error          next_directory_error            = MELPHIG_CONST_MPHIG_ERROR;
    struct mphig_list           file_names                      = MELPHIG_CONST_MPHIG_LIST;
    struct mphig_list_iterator  file_names_iterator             = { &file_names, NULL };
    char*                       file_name                       = NULL;
    mphig                       next_file_name_ret              = MELPHIG_FAIL;
    struct mphig_error          next_file_name_error            = MELPHIG_CONST_MPHIG_ERROR;
    struct mphig_list           paths                           = MELPHIG_CONST_MPHIG_LIST;
    struct mphig_list_iterator  paths_iterator                  = { &paths, NULL };
    mphig                       next_path_ret                   = MELPHIG_FAIL;
    struct mphig_error          next_path_error                 = MELPHIG_CONST_MPHIG_ERROR;
    char                        path_buffer[MELPHIG_BUFFER_SIZE];
    int                         path_buffer_len                 = 0;
    char*                       path                            = NULL;
    char*                       full_path                       = NULL;


    int                         extension_index                 = 0;
    char*                       extension                       = NULL;
    char*                       module_extensions[]             = {
        ".so",
        ".dll",
    };

    // NULL checks
    if( Mphig_Module_Load == NULL || Modules == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Mphig_Module_Load or Modules is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    // Assume ok
    ret = MELPHIG_OK;

    file_names_iterator.current_node = NULL;
    while( MELPHIG_OK == ( next_directory_ret = mphig_list_next( &plugins_directories_iterator,
                                                                 &directory,
                                                                 &next_directory_error ) ) &&
           ret == MELPHIG_OK )
    {
        if( MELPHIG_FAIL == ( ret = mphig_directory_list( directory,
                                                          &file_names,
                                                          Error ) ) )
        {
            break;
        }
        if( ret == MELPHIG_OK )
        {
            while( ret == MELPHIG_OK &&
                   MELPHIG_OK == ( next_file_name_ret = mphig_list_next( &file_names_iterator,
                                                                         &file_name,
                                                                         &next_file_name_error ) ) )
            {
                memset( path_buffer,
                        0x00,
                        MELPHIG_BUFFER_SIZE );

                snprintf( path_buffer,
                          MELPHIG_BUFFER_SIZE,
                          "%s/%s",
                          directory,
                          file_name );

                // Reset
                path = NULL;

                path_buffer_len = strnlen( path_buffer, MELPHIG_BUFFER_SIZE-1);

                if( MELPHIG_OK == ( ret = mphig_malloc( path_buffer_len+1,
                                                        &path,
                                                        Error ) ) )
                {
                    memset( path,
                            0x00,
                            path_buffer_len+1 );

                    memcpy( path,
                            path_buffer,
                            path_buffer_len );

                    ret = mphig_list_push( &paths,
                                           path,
                                           Error );

                    path = NULL;
                }
            }
            if( next_file_name_ret == MELPHIG_FAIL &&
                next_file_name_error.error_type == mphig_system_error )
            {
                if( Error != NULL )
                    *Error = next_file_name_error;

                ret = MELPHIG_FAIL;
            }
            if( ret == MELPHIG_OK )
            {
                file_names_iterator.current_node = file_names.last;
            }
        }
    } // while

    if( next_directory_ret == MELPHIG_FAIL &&
        next_directory_error.error_type == mphig_system_error )
    {
        if( Error != NULL )
            *Error = next_directory_error;

        ret = MELPHIG_FAIL;
    }

    if( ret == MELPHIG_OK )
    {
        while( MELPHIG_OK == ( next_path_ret = mphig_list_next( &paths_iterator,
                                                                &full_path,
                                                                &next_path_error ) ) )
        {
            // Silently skip the file when it doesnt have one of the plugin extensions
            extension = NULL;

            for( extension_index = 0; ret == MELPHIG_OK && extension_index < sizeof( module_extensions ) / sizeof( char* ) && extension == NULL; extension_index++ )
            {
                ret = mphig_strnstr( full_path,
                                     module_extensions[extension_index],
                                     MELPHIG_BUFFER_SIZE,
                                     &extension,
                                     Error );
            }
            if( ret == MELPHIG_OK )
            {
                if( extension == NULL )
                {
                    continue;
                }

                if( MELPHIG_FAIL == ( ret = Mphig_Module_Load( full_path,
                                                               Modules,
                                                               Error ) ) )
                {
                    break;
                }
            }
        } // while
    }

    if( next_path_ret == MELPHIG_FAIL &&
        next_path_error.error_type == mphig_system_error )
    {
        if( Error != NULL )
            *Error = next_path_error;

        ret = MELPHIG_FAIL;
    }

    // clean up
    mphig_destroy_directory_list( &paths, NULL );
    mphig_destroy_directory_list( &file_names, NULL );

    // clean up on error
    if( ret == MELPHIG_FAIL )
    {
        mphig_free( &path, NULL );

    }

    return ret;
}


