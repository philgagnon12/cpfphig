#include "melphig/melphig.h"
#include "melphig/list.h"
#include "melphig/malloc.h"

mphig
REAL(mphig_list_push)( struct mphig_list*                      List,
                       void*                                   Item,
                       MELPHIG_OPTIONAL struct mphig_error*    Error )
{
    struct mphig_list_node* node     = NULL;
    struct mphig_list_node* previous = NULL;

    if( List == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "List is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    if( MELPHIG_FAIL == mphig_malloc( sizeof( struct mphig_list_node ),
                                      &node,
                                      Error ) )
    {
        return MELPHIG_FAIL;
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

    return MELPHIG_OK;
}
