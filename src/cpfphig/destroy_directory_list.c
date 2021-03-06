#include "cpfphig/cpfphig.h"
#include "cpfphig/destroy_directory_list.h"
#include "cpfphig/list.h"
#include "cpfphig/free.h"

cpfphig
CPFPHIG_REAL(cpfphig_destroy_directory_list)( struct cpfphig_list*                    File_Names,
                                              CPFPHIG_OPTIONAL struct cpfphig_error*  Error )
{
    cpfphig                 ret             = CPFPHIG_FAIL;
    char*                   file_name       = NULL;
    struct cpfphig_error    shift_error     = CPFPHIG_CONST_CPFPHIG_ERROR;

    if( File_Names == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "File_Names is NULL", Error );

        return CPFPHIG_FAIL;
    }

    // Assume ok
    shift_error.error_type = cpfphig_ok;

    // TODO File_Names should be renamed to Files_Name accross the board
    while( CPFPHIG_OK == ( ret = cpfphig_list_shift( File_Names,
                                                     &file_name,
                                                     &shift_error ) ) )
    {
        cpfphig_free( &file_name, NULL );
    }

    // Hand out error when its an error different than shifting from empty list
    if( shift_error.error_type != cpfphig_user_error )
    {
        if( Error != NULL )
            *Error = shift_error;

        return ret;
    }

    return CPFPHIG_OK;
}

