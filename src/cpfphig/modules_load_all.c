#include "cpfphig/cpfphig.h"
#include "cpfphig/modules_load_all.h"
#include "cpfphig/directory_list.h"
#include "cpfphig/malloc.h"
#include "cpfphig/free.h"
#include "cpfphig/destroy_directory_list.h"
#include "cpfphig/strnstr.h"

#include "string.h"
#include "stdio.h"

#define CPFPHIG_BUFFER_SIZE ( 0x04FF )

cpfphig
cpfphig_modules_load_all( struct cpfphig_list                         Modules_Directories,
                        cpfphig_module_load_symbol*                 Mphig_Module_Load,
                        struct cpfphig_list*                        Modules,
                        CPFPHIG_OPTIONAL struct cpfphig_error*      Error )
{
    cpfphig                       ret                             = CPFPHIG_FAIL;
    struct cpfphig_list_iterator  plugins_directories_iterator    = { &Modules_Directories, NULL };
    char*                       directory                       = NULL;
    cpfphig                       next_directory_ret              = CPFPHIG_FAIL;
    struct cpfphig_error          next_directory_error            = CPFPHIG_CONST_CPFPHIG_ERROR;
    struct cpfphig_list           file_names                      = CPFPHIG_CONST_CPFPHIG_LIST;
    struct cpfphig_list_iterator  file_names_iterator             = { &file_names, NULL };
    char*                       file_name                       = NULL;
    cpfphig                       next_file_name_ret              = CPFPHIG_FAIL;
    struct cpfphig_error          next_file_name_error            = CPFPHIG_CONST_CPFPHIG_ERROR;
    struct cpfphig_list           paths                           = CPFPHIG_CONST_CPFPHIG_LIST;
    struct cpfphig_list_iterator  paths_iterator                  = { &paths, NULL };
    cpfphig                       next_path_ret                   = CPFPHIG_FAIL;
    struct cpfphig_error          next_path_error                 = CPFPHIG_CONST_CPFPHIG_ERROR;
    char                        path_buffer[CPFPHIG_BUFFER_SIZE];
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
            cpfphig_error_message(cpfphig_system_error, "Mphig_Module_Load or Modules is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    // Assume ok
    ret = CPFPHIG_OK;

    file_names_iterator.current_node = NULL;
    while( CPFPHIG_OK == ( next_directory_ret = cpfphig_list_next( &plugins_directories_iterator,
                                                                 &directory,
                                                                 &next_directory_error ) ) &&
           ret == CPFPHIG_OK )
    {
        if( CPFPHIG_FAIL == ( ret = cpfphig_directory_list( directory,
                                                          &file_names,
                                                          Error ) ) )
        {
            break;
        }
        if( ret == CPFPHIG_OK )
        {
            while( ret == CPFPHIG_OK &&
                   CPFPHIG_OK == ( next_file_name_ret = cpfphig_list_next( &file_names_iterator,
                                                                         &file_name,
                                                                         &next_file_name_error ) ) )
            {
                memset( path_buffer,
                        0x00,
                        CPFPHIG_BUFFER_SIZE );

                snprintf( path_buffer,
                          CPFPHIG_BUFFER_SIZE,
                          "%s/%s",
                          directory,
                          file_name );

                // Reset
                path = NULL;

                path_buffer_len = strnlen( path_buffer, CPFPHIG_BUFFER_SIZE-1);

                if( CPFPHIG_OK == ( ret = cpfphig_malloc( path_buffer_len+1,
                                                        &path,
                                                        Error ) ) )
                {
                    memset( path,
                            0x00,
                            path_buffer_len+1 );

                    memcpy( path,
                            path_buffer,
                            path_buffer_len );

                    ret = cpfphig_list_push( &paths,
                                           path,
                                           Error );

                    path = NULL;
                }
            }
            if( next_file_name_ret == CPFPHIG_FAIL &&
                next_file_name_error.error_type == cpfphig_system_error )
            {
                if( Error != NULL )
                    *Error = next_file_name_error;

                ret = CPFPHIG_FAIL;
            }
            if( ret == CPFPHIG_OK )
            {
                file_names_iterator.current_node = file_names.last;
            }
        }
    } // while

    if( next_directory_ret == CPFPHIG_FAIL &&
        next_directory_error.error_type == cpfphig_system_error )
    {
        if( Error != NULL )
            *Error = next_directory_error;

        ret = CPFPHIG_FAIL;
    }

    if( ret == CPFPHIG_OK )
    {
        while( CPFPHIG_OK == ( next_path_ret = cpfphig_list_next( &paths_iterator,
                                                                &full_path,
                                                                &next_path_error ) ) )
        {
            // Silently skip the file when it doesnt have one of the plugin extensions
            extension = NULL;

            for( extension_index = 0; ret == CPFPHIG_OK && extension_index < sizeof( module_extensions ) / sizeof( char* ) && extension == NULL; extension_index++ )
            {
                ret = cpfphig_strnstr( full_path,
                                     module_extensions[extension_index],
                                     CPFPHIG_BUFFER_SIZE,
                                     &extension,
                                     Error );
            }
            if( ret == CPFPHIG_OK )
            {
                if( extension == NULL )
                {
                    continue;
                }

                if( CPFPHIG_FAIL == ( ret = Mphig_Module_Load( full_path,
                                                               Modules,
                                                               Error ) ) )
                {
                    break;
                }
            }
        } // while
    }

    if( next_path_ret == CPFPHIG_FAIL &&
        next_path_error.error_type == cpfphig_system_error )
    {
        if( Error != NULL )
            *Error = next_path_error;

        ret = CPFPHIG_FAIL;
    }

    // clean up
    cpfphig_destroy_directory_list( &paths, NULL );
    cpfphig_destroy_directory_list( &file_names, NULL );

    // clean up on error
    if( ret == CPFPHIG_FAIL )
    {
        cpfphig_free( &path, NULL );

    }

    return ret;
}


