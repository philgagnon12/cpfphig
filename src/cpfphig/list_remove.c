#include "cpfphig/cpfphig.h"
#include "cpfphig/list.h"
#include "cpfphig/malloc.h"

cpfphig
REAL(cpfphig_list_remove)( struct cpfphig_list*                      List,
                         void*                                   Item,
                         CPFPHIG_OPTIONAL struct cpfphig_error*    Error )
{
    cpfphig                       ret                     = CPFPHIG_FAIL;
    void*                       item                    = NULL;
    void*                       removed_item            = NULL;
    cpfphig                       shift_ret               = CPFPHIG_FAIL;
    struct cpfphig_error          shift_error             = CPFPHIG_CONST_CPFPHIG_ERROR;
    struct cpfphig_list           reordered_list          = CPFPHIG_CONST_CPFPHIG_LIST;

    if( List == NULL || Item == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "List or Item is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    // Assume ok
    ret = CPFPHIG_OK;

    while( ret == CPFPHIG_OK &&
           CPFPHIG_OK == ( shift_ret = cpfphig_list_shift( List,
                                                         &item,
                                                         &shift_error ) ) )
    {
        if( Item == item )
        {
            removed_item = item;
            continue;
        }

        ret = cpfphig_list_push( &reordered_list,
                               item,
                               Error );
    }
    if( shift_ret == CPFPHIG_FAIL &&
        shift_error.error_type == cpfphig_system_error )
    {
        if( Error != NULL )
            *Error = shift_error;

        return CPFPHIG_FAIL;
    }


    if( ret == CPFPHIG_OK )
    {
        *List = reordered_list;

        if( removed_item == NULL )
        {
            if( Error != NULL )
                cpfphig_error_message(cpfphig_user_error, "Item was not removed from the List", Error, __FILE__, __FUNCTION__, __LINE__ );

            ret = CPFPHIG_FAIL;
        }
    }

    return ret;
}
