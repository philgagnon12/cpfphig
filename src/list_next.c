#include "melphig/melphig.h"
#include "melphig/list.h"

mphig
REAL(mphig_list_next)( struct mphig_list_iterator* const       List_Iterator,
                       void*                                   Item,
                       MELPHIG_OPTIONAL struct mphig_error*    Error )
{
    void*                   item            = NULL;
    struct mphig_list_node* current_node    = NULL;

    if( List_Iterator == NULL || Item == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "List_Iterator or Item is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    if( List_Iterator->list == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "List_Iterator->list is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    if( List_Iterator->current_node == NULL )
    {
        current_node = List_Iterator->list->first;
    }
    else
    {
        current_node = List_Iterator->current_node->next;
    }

    List_Iterator->current_node = current_node;

    if( current_node == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_user_error, "End of list has been reached", Error, __FILE__, __FUNCTION__, __LINE__ );


        return MELPHIG_FAIL;
    }

    // System error, we do not allow NULL items in the list when using mphig_list_next
    // It is discouraged to iterate through list manually
    if( current_node->item == NULL )
    {
            if( Error != NULL )
                mphig_error_message(mphig_system_error, "current_node->item is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

            return MELPHIG_FAIL;
    }

    *(void**)Item = current_node->item;

    return MELPHIG_OK;
}

