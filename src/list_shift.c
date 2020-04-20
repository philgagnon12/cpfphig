#include "melphig/melphig.h"
#include "melphig/list.h"
#include "melphig/free.h"

fphig
REAL(fphig_list_shift)( struct fphig_list*                      List,
                        void*                                   Item,
                        FPHIG_OPTIONAL struct fphig_error*    Error )
{
    void*                   item    = NULL;
    struct fphig_list_node* node    = NULL;

    if( List == NULL || Item == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "List or Item is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    node = List->first;

    if( node == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_user_error, "List is empty", Error, __FILE__, __FUNCTION__, __LINE__ );

        *(void**)Item = NULL;

        return FPHIG_FAIL;
    }

        // Assign the new first item
    List->first = node->next;


    item = node->item;

    if( FPHIG_FAIL == fphig_free( &node,
                                    Error ) )
    {
        return FPHIG_FAIL;
    }

    *(void**)Item = item;

        // Re arrange list
    if( List->first == NULL )
    {
        List->last = NULL;
    }
    else
    {
        if( List->first->next == NULL )
        {
            List->last = List->first;
        }

        List->first->previous = NULL;
    }

    return FPHIG_OK;
}

