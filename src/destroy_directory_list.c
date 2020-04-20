#include "melphig/melphig.h"
#include "melphig/destroy_directory_list.h"
#include "melphig/list.h"
#include "melphig/free.h"

mphig
REAL(mphig_destroy_directory_list)( struct mphig_list*                    File_Names,
                                    MELPHIG_OPTIONAL struct mphig_error*  Error )
{
    mphig                           ret             = MELPHIG_FAIL;
    char*                           file_name       = NULL;
    struct mphig_error              shift_error     = MELPHIG_CONST_MPHIG_ERROR;

    if( File_Names == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "File_Names is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    // TODO File_Names should be renamed to Files_Name accross the board
    while( MELPHIG_OK == ( ret = mphig_list_shift( File_Names,
                                                   &file_name,
                                                   &shift_error ) ) )
    {
        mphig_free( &file_name, NULL );
    }

    // Hand out error when its an error different than shifting from empty list
    if( shift_error.error_type != mphig_user_error )
    {
        if( Error != NULL )
            *Error = shift_error;

        return ret;
    }

    return MELPHIG_OK;
}

