#include "fphig/fphig.h"
#include "fphig/modules_load_all.h"
#include "fphig/directory_list.h"
#include "fphig/malloc.h"
#include "fphig/free.h"
#include "fphig/destroy_directory_list.h"
#include "fphig/strnstr.h"

#include "string.h"
#include "stdio.h"

#define FPHIG_BUFFER_SIZE ( 0x04FF )

fphig
fphig_modules_load_all( struct fphig_list                         Modules_Directories,
                        fphig_module_load_symbol*                 Mphig_Module_Load,
                        struct fphig_list*                        Modules,
                        FPHIG_OPTIONAL struct fphig_error*      Error )
{
    fphig                       ret                             = FPHIG_FAIL;
    struct fphig_list_iterator  plugins_directories_iterator    = { &Modules_Directories, NULL };
    char*                       directory                       = NULL;
    fphig                       next_directory_ret              = FPHIG_FAIL;
    struct fphig_error          next_directory_error            = FPHIG_CONST_FPHIG_ERROR;
    struct fphig_list           file_names                      = FPHIG_CONST_FPHIG_LIST;
    struct fphig_list_iterator  file_names_iterator             = { &file_names, NULL };
    char*                       file_name                       = NULL;
    fphig                       next_file_name_ret              = FPHIG_FAIL;
    struct fphig_error          next_file_name_error            = FPHIG_CONST_FPHIG_ERROR;
    struct fphig_list           paths                           = FPHIG_CONST_FPHIG_LIST;
    struct fphig_list_iterator  paths_iterator                  = { &paths, NULL };
    fphig                       next_path_ret                   = FPHIG_FAIL;
    struct fphig_error          next_path_error                 = FPHIG_CONST_FPHIG_ERROR;
    char                        path_buffer[FPHIG_BUFFER_SIZE];
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
            fphig_error_message(fphig_system_error, "Mphig_Module_Load or Modules is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    // Assume ok
    ret = FPHIG_OK;

    file_names_iterator.current_node = NULL;
    while( FPHIG_OK == ( next_directory_ret = fphig_list_next( &plugins_directories_iterator,
                                                                 &directory,
                                                                 &next_directory_error ) ) &&
           ret == FPHIG_OK )
    {
        if( FPHIG_FAIL == ( ret = fphig_directory_list( directory,
                                                          &file_names,
                                                          Error ) ) )
        {
            break;
        }
        if( ret == FPHIG_OK )
        {
            while( ret == FPHIG_OK &&
                   FPHIG_OK == ( next_file_name_ret = fphig_list_next( &file_names_iterator,
                                                                         &file_name,
                                                                         &next_file_name_error ) ) )
            {
                memset( path_buffer,
                        0x00,
                        FPHIG_BUFFER_SIZE );

                snprintf( path_buffer,
                          FPHIG_BUFFER_SIZE,
                          "%s/%s",
                          directory,
                          file_name );

                // Reset
                path = NULL;

                path_buffer_len = strnlen( path_buffer, FPHIG_BUFFER_SIZE-1);

                if( FPHIG_OK == ( ret = fphig_malloc( path_buffer_len+1,
                                                        &path,
                                                        Error ) ) )
                {
                    memset( path,
                            0x00,
                            path_buffer_len+1 );

                    memcpy( path,
                            path_buffer,
                            path_buffer_len );

                    ret = fphig_list_push( &paths,
                                           path,
                                           Error );

                    path = NULL;
                }
            }
            if( next_file_name_ret == FPHIG_FAIL &&
                next_file_name_error.error_type == fphig_system_error )
            {
                if( Error != NULL )
                    *Error = next_file_name_error;

                ret = FPHIG_FAIL;
            }
            if( ret == FPHIG_OK )
            {
                file_names_iterator.current_node = file_names.last;
            }
        }
    } // while

    if( next_directory_ret == FPHIG_FAIL &&
        next_directory_error.error_type == fphig_system_error )
    {
        if( Error != NULL )
            *Error = next_directory_error;

        ret = FPHIG_FAIL;
    }

    if( ret == FPHIG_OK )
    {
        while( FPHIG_OK == ( next_path_ret = fphig_list_next( &paths_iterator,
                                                                &full_path,
                                                                &next_path_error ) ) )
        {
            // Silently skip the file when it doesnt have one of the plugin extensions
            extension = NULL;

            for( extension_index = 0; ret == FPHIG_OK && extension_index < sizeof( module_extensions ) / sizeof( char* ) && extension == NULL; extension_index++ )
            {
                ret = fphig_strnstr( full_path,
                                     module_extensions[extension_index],
                                     FPHIG_BUFFER_SIZE,
                                     &extension,
                                     Error );
            }
            if( ret == FPHIG_OK )
            {
                if( extension == NULL )
                {
                    continue;
                }

                if( FPHIG_FAIL == ( ret = Mphig_Module_Load( full_path,
                                                               Modules,
                                                               Error ) ) )
                {
                    break;
                }
            }
        } // while
    }

    if( next_path_ret == FPHIG_FAIL &&
        next_path_error.error_type == fphig_system_error )
    {
        if( Error != NULL )
            *Error = next_path_error;

        ret = FPHIG_FAIL;
    }

    // clean up
    fphig_destroy_directory_list( &paths, NULL );
    fphig_destroy_directory_list( &file_names, NULL );

    // clean up on error
    if( ret == FPHIG_FAIL )
    {
        fphig_free( &path, NULL );

    }

    return ret;
}


