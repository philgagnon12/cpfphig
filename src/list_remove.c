#include "melphig/melphig.h"
#include "melphig/list.h"
#include "melphig/malloc.h"

mphig
REAL(mphig_list_remove)( struct mphig_list*                      List,
                         void*                                   Item,
                         MELPHIG_OPTIONAL struct mphig_error*    Error )
{
    mphig                       ret                     = MELPHIG_FAIL;
    void*                       item                    = NULL;
    void*                       removed_item            = NULL;
    mphig                       shift_ret               = MELPHIG_FAIL;
    struct mphig_error          shift_error             = MELPHIG_CONST_MPHIG_ERROR;
    struct mphig_list           reordered_list          = MELPHIG_CONST_MPHIG_LIST;

    if( List == NULL || Item == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "List or Item is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    // Assume ok
    ret = MELPHIG_OK;

    while( ret == MELPHIG_OK &&
           MELPHIG_OK == ( shift_ret = mphig_list_shift( List,
                                                         &item,
                                                         &shift_error ) ) )
    {
        if( Item == item )
        {
            removed_item = item;
            continue;
        }

        ret = mphig_list_push( &reordered_list,
                               item,
                               Error );
    }
    if( shift_ret == MELPHIG_FAIL &&
        shift_error.error_type == mphig_system_error )
    {
        if( Error != NULL )
            *Error = shift_error;

        return MELPHIG_FAIL;
    }


    if( ret == MELPHIG_OK )
    {
        *List = reordered_list;

        if( removed_item == NULL )
        {
            if( Error != NULL )
                mphig_error_message(mphig_user_error, "Item was not removed from the List", Error, __FILE__, __FUNCTION__, __LINE__ );

            ret = MELPHIG_FAIL;
        }
    }

    return ret;
}
