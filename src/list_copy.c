#include "melphig/melphig.h"
#include "melphig/list.h"

mphig
REAL(mphig_list_copy)( struct mphig_list                       List,
                       struct mphig_list*                      List_Copy,
                       MELPHIG_OPTIONAL struct mphig_error*    Error )
{
    mphig                       ret             = MELPHIG_FAIL;
    struct mphig_list_iterator  list_iterator   = MELPHIG_CONST_MPHIG_LIST_ITERATOR;
    void*                       item            = NULL;
    mphig                       next_ret        = MELPHIG_FAIL;
    struct mphig_error          next_error      = MELPHIG_CONST_MPHIG_ERROR;
    struct mphig_list           list_copy       = MELPHIG_CONST_MPHIG_LIST;

    if( List_Copy == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "List_Copy is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    list_iterator.list          = &List;
    list_iterator.current_node  = NULL;

    // Assume ok
    ret = MELPHIG_OK;

    while( MELPHIG_OK == ( next_ret = mphig_list_next( &list_iterator,
                                                       &item,
                                                       &next_error ) ) &&
           ret == MELPHIG_OK )
    {
        ret = mphig_list_push( &list_copy,
                               item,
                               Error );
    }
    if( next_ret == MELPHIG_FAIL &&
        next_error.error_type == mphig_system_error )
    {
        if( Error != NULL )
            *Error = next_error;

        ret = MELPHIG_FAIL;
    }

    if( ret == MELPHIG_OK )
    {
        *List_Copy = list_copy;
    }

    return ret;
}
