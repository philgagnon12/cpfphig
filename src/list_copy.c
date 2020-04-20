#include "fphig/fphig.h"
#include "fphig/list.h"

fphig
REAL(fphig_list_copy)( struct fphig_list                       List,
                       struct fphig_list*                      List_Copy,
                       FPHIG_OPTIONAL struct fphig_error*    Error )
{
    fphig                       ret             = FPHIG_FAIL;
    struct fphig_list_iterator  list_iterator   = FPHIG_CONST_MPHIG_LIST_ITERATOR;
    void*                       item            = NULL;
    fphig                       next_ret        = FPHIG_FAIL;
    struct fphig_error          next_error      = FPHIG_CONST_MPHIG_ERROR;
    struct fphig_list           list_copy       = FPHIG_CONST_MPHIG_LIST;

    if( List_Copy == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "List_Copy is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    list_iterator.list          = &List;
    list_iterator.current_node  = NULL;

    // Assume ok
    ret = FPHIG_OK;

    while( FPHIG_OK == ( next_ret = fphig_list_next( &list_iterator,
                                                       &item,
                                                       &next_error ) ) &&
           ret == FPHIG_OK )
    {
        ret = fphig_list_push( &list_copy,
                               item,
                               Error );
    }
    if( next_ret == FPHIG_FAIL &&
        next_error.error_type == fphig_system_error )
    {
        if( Error != NULL )
            *Error = next_error;

        ret = FPHIG_FAIL;
    }

    if( ret == FPHIG_OK )
    {
        *List_Copy = list_copy;
    }

    return ret;
}
