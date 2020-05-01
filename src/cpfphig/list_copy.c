#include "cpfphig/cpfphig.h"
#include "cpfphig/list.h"

cpfphig
CPFPHIG_REAL(cpfphig_list_copy)( struct cpfphig_list                       List,
                       struct cpfphig_list*                      List_Copy,
                       CPFPHIG_OPTIONAL struct cpfphig_error*    Error )
{
    cpfphig                       ret             = CPFPHIG_FAIL;
    struct cpfphig_list_iterator  list_iterator   = CPFPHIG_CONST_CPFPHIG_LIST_ITERATOR;
    void*                       item            = NULL;
    cpfphig                       next_ret        = CPFPHIG_FAIL;
    struct cpfphig_error          next_error      = CPFPHIG_CONST_CPFPHIG_ERROR;
    struct cpfphig_list           list_copy       = CPFPHIG_CONST_CPFPHIG_LIST;

    if( List_Copy == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "List_Copy is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    list_iterator.list          = &List;
    list_iterator.current_node  = NULL;

    // Assume ok
    ret = CPFPHIG_OK;

    while( CPFPHIG_OK == ( next_ret = cpfphig_list_next( &list_iterator,
                                                       &item,
                                                       &next_error ) ) &&
           ret == CPFPHIG_OK )
    {
        ret = cpfphig_list_push( &list_copy,
                               item,
                               Error );
    }
    if( next_ret == CPFPHIG_FAIL &&
        next_error.error_type == cpfphig_system_error )
    {
        if( Error != NULL )
            *Error = next_error;

        ret = CPFPHIG_FAIL;
    }

    if( ret == CPFPHIG_OK )
    {
        *List_Copy = list_copy;
    }

    return ret;
}
