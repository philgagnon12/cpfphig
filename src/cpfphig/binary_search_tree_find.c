#include "cpfphig/cpfphig.h"
#include "cpfphig/binary_search_tree.h"


cpfphig
cpfphig_binary_search_tree_find( struct cpfphig_binary_search_tree*         Tree,
                                 void*                                      Key,
                                 void**                                     Item,
                                 cpfphig_binary_search_tree_compare_symbol* Compare_Symbol,
                                 CPFPHIG_OPTIONAL struct cpfphig_error*     Error )
{
    cpfphig ret                         = CPFPHIG_FAIL;
    int     compare                     = 0;

    struct cpfphig_binary_search_tree** tree                = NULL;


    // NULL checks
    if( Tree == NULL || Key == NULL || Item == NULL || Compare_Symbol == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message( cpfphig_system_error, "Tree, Key, Item or Compare_Symbol is NULL", Error );

        return CPFPHIG_FAIL;
    }

    if( Tree->key == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message( cpfphig_system_error, "Tree->key is NULL", Error );

        return CPFPHIG_FAIL;
    }

    ret  = Compare_Symbol( Key, Tree->key, &compare, Error );

    if( ret == CPFPHIG_OK )
    {
        if( compare < 0 )
        {
            tree = &Tree->left;
        }
        else if( compare > 0 )
        {
            tree = &Tree->right;
        }
        else
        {
            *Item = Tree->item;

            return CPFPHIG_OK;
        }
    }

    if( ret == CPFPHIG_OK )
    {
        if( *tree == NULL )
        {
             if( Error != NULL )
                cpfphig_error_message( cpfphig_user_error, "Key not found", Error );

             ret = CPFPHIG_FAIL;
        }
        else
        {
            ret = cpfphig_binary_search_tree_find( *tree,
                                                   Key,
                                                   Item,
                                                   Compare_Symbol,
                                                   Error );
        }
    }

    return ret;
}



