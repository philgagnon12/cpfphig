#include "cpfphig/cpfphig.h"
#include "cpfphig/list.h"
#include "cpfphig/malloc.h"

cpfphig
REAL(cpfphig_list_push)( struct cpfphig_list*                      List,
                       void*                                   Item,
                       CPFPHIG_OPTIONAL struct cpfphig_error*    Error )
{
    struct cpfphig_list_node* node     = NULL;
    struct cpfphig_list_node* previous = NULL;

    if( List == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "List is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    if( CPFPHIG_FAIL == cpfphig_malloc( sizeof( struct cpfphig_list_node ),
                                      &node,
                                      Error ) )
    {
        return CPFPHIG_FAIL;
    }

    node->item = Item;

    if( List->first == NULL || List->last == NULL )
    {
        List->first = node;
        List->last  = node;

        node->next      = NULL;
        node->previous  = NULL;
    }
    else
    {
        previous = List->last;

        previous->next = node;

        node->next     = NULL;
        node->previous = previous;

        List->last = node;
    }

    return CPFPHIG_OK;
}
