#include "cpfphig/cpfphig.h"
#include "cpfphig/list.h"
#include "cpfphig/free.h"

cpfphig
CPFPHIG_REAL(cpfphig_list_shift)( struct cpfphig_list*                      List,
                        void*                                   Item,
                        CPFPHIG_OPTIONAL struct cpfphig_error*    Error )
{
    void*                   item    = NULL;
    struct cpfphig_list_node* node    = NULL;

    if( List == NULL || Item == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "List or Item is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    node = List->first;

    if( node == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_user_error, "List is empty", Error, __FILE__, __FUNCTION__, __LINE__ );

        *(void**)Item = NULL;

        return CPFPHIG_FAIL;
    }

        // Assign the new first item
    List->first = node->next;


    item = node->item;

    if( CPFPHIG_FAIL == cpfphig_free( &node,
                                    Error ) )
    {
        return CPFPHIG_FAIL;
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

    return CPFPHIG_OK;
}

