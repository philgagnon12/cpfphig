#include "melphig/melphig.h"
#include "melphig/list.h"
#include "melphig/free.h"

mphig
REAL(mphig_list_shift)( struct mphig_list*                      List,
                        void*                                   Item,
                        MELPHIG_OPTIONAL struct mphig_error*    Error )
{
    void*                   item    = NULL;
    struct mphig_list_node* node    = NULL;

    if( List == NULL || Item == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "List or Item is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    node = List->first;

    if( node == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_user_error, "List is empty", Error, __FILE__, __FUNCTION__, __LINE__ );

        *(void**)Item = NULL;

        return MELPHIG_FAIL;
    }

        // Assign the new first item
    List->first = node->next;


    item = node->item;

    if( MELPHIG_FAIL == mphig_free( &node,
                                    Error ) )
    {
        return MELPHIG_FAIL;
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

    return MELPHIG_OK;
}

