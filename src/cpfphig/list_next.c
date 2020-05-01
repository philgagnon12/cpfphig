#include "cpfphig/cpfphig.h"
#include "cpfphig/list.h"

cpfphig
CPFPHIG_REAL(cpfphig_list_next)( struct cpfphig_list_iterator* const       List_Iterator,
                       void*                                   Item,
                       CPFPHIG_OPTIONAL struct cpfphig_error*    Error )
{
    void*                   item            = NULL;
    struct cpfphig_list_node* current_node    = NULL;

    if( List_Iterator == NULL || Item == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "List_Iterator or Item is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    if( List_Iterator->list == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "List_Iterator->list is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
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
            cpfphig_error_message(cpfphig_user_error, "End of list has been reached", Error, __FILE__, __FUNCTION__, __LINE__ );


        return CPFPHIG_FAIL;
    }

    // System error, we do not allow NULL items in the list when using cpfphig_list_next
    // It is discouraged to iterate through list manually
    if( current_node->item == NULL )
    {
            if( Error != NULL )
                cpfphig_error_message(cpfphig_system_error, "current_node->item is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

            return CPFPHIG_FAIL;
    }

    *(void**)Item = current_node->item;

    return CPFPHIG_OK;
}

