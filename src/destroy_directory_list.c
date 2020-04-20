#include "melphig/melphig.h"
#include "melphig/destroy_directory_list.h"
#include "melphig/list.h"
#include "melphig/free.h"

fphig
REAL(fphig_destroy_directory_list)( struct fphig_list*                    File_Names,
                                    FPHIG_OPTIONAL struct fphig_error*  Error )
{
    fphig                           ret             = FPHIG_FAIL;
    char*                           file_name       = NULL;
    struct fphig_error              shift_error     = FPHIG_CONST_MPHIG_ERROR;

    if( File_Names == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "File_Names is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    // TODO File_Names should be renamed to Files_Name accross the board
    while( FPHIG_OK == ( ret = fphig_list_shift( File_Names,
                                                   &file_name,
                                                   &shift_error ) ) )
    {
        fphig_free( &file_name, NULL );
    }

    // Hand out error when its an error different than shifting from empty list
    if( shift_error.error_type != fphig_user_error )
    {
        if( Error != NULL )
            *Error = shift_error;

        return ret;
    }

    return FPHIG_OK;
}

