#include "fphig/fphig.h"
#include "fphig/list.h"
#include "fphig/malloc.h"

fphig
REAL(fphig_list_remove)( struct fphig_list*                      List,
                         void*                                   Item,
                         FPHIG_OPTIONAL struct fphig_error*    Error )
{
    fphig                       ret                     = FPHIG_FAIL;
    void*                       item                    = NULL;
    void*                       removed_item            = NULL;
    fphig                       shift_ret               = FPHIG_FAIL;
    struct fphig_error          shift_error             = FPHIG_CONST_FPHIG_ERROR;
    struct fphig_list           reordered_list          = FPHIG_CONST_FPHIG_LIST;

    if( List == NULL || Item == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "List or Item is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    // Assume ok
    ret = FPHIG_OK;

    while( ret == FPHIG_OK &&
           FPHIG_OK == ( shift_ret = fphig_list_shift( List,
                                                         &item,
                                                         &shift_error ) ) )
    {
        if( Item == item )
        {
            removed_item = item;
            continue;
        }

        ret = fphig_list_push( &reordered_list,
                               item,
                               Error );
    }
    if( shift_ret == FPHIG_FAIL &&
        shift_error.error_type == fphig_system_error )
    {
        if( Error != NULL )
            *Error = shift_error;

        return FPHIG_FAIL;
    }


    if( ret == FPHIG_OK )
    {
        *List = reordered_list;

        if( removed_item == NULL )
        {
            if( Error != NULL )
                fphig_error_message(fphig_user_error, "Item was not removed from the List", Error, __FILE__, __FUNCTION__, __LINE__ );

            ret = FPHIG_FAIL;
        }
    }

    return ret;
}
