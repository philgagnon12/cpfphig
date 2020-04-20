#include "fphig/fphig.h"
#include "fphig/list.h"
#include "fphig/malloc.h"

fphig
REAL(fphig_list_push)( struct fphig_list*                      List,
                       void*                                   Item,
                       FPHIG_OPTIONAL struct fphig_error*    Error )
{
    struct fphig_list_node* node     = NULL;
    struct fphig_list_node* previous = NULL;

    if( List == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "List is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    if( FPHIG_FAIL == fphig_malloc( sizeof( struct fphig_list_node ),
                                      &node,
                                      Error ) )
    {
        return FPHIG_FAIL;
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

    return FPHIG_OK;
}
