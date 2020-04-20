#include "fphig/fphig.h"
#include "fphig/list.h"

fphig
REAL(fphig_list_next)( struct fphig_list_iterator* const       List_Iterator,
                       void*                                   Item,
                       FPHIG_OPTIONAL struct fphig_error*    Error )
{
    void*                   item            = NULL;
    struct fphig_list_node* current_node    = NULL;

    if( List_Iterator == NULL || Item == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "List_Iterator or Item is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    if( List_Iterator->list == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "List_Iterator->list is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
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
            fphig_error_message(fphig_user_error, "End of list has been reached", Error, __FILE__, __FUNCTION__, __LINE__ );


        return FPHIG_FAIL;
    }

    // System error, we do not allow NULL items in the list when using fphig_list_next
    // It is discouraged to iterate through list manually
    if( current_node->item == NULL )
    {
            if( Error != NULL )
                fphig_error_message(fphig_system_error, "current_node->item is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

            return FPHIG_FAIL;
    }

    *(void**)Item = current_node->item;

    return FPHIG_OK;
}

