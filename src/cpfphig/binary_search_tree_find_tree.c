#include "cpfphig/cpfphig.h"
#include "cpfphig/binary_search_tree.h"


cpfphig
cpfphig_binary_search_tree_find_tree( struct cpfphig_binary_search_tree*         Tree,
                                      void*                                      Key,
                                      struct cpfphig_binary_search_tree**        Tree_At_Key,
                                      cpfphig_binary_search_tree_compare_symbol* Compare_Symbol,
                                      CPFPHIG_OPTIONAL struct cpfphig_error*     Error )
{
    cpfphig ret                         = CPFPHIG_FAIL;
    int     compare                     = 0;

    struct cpfphig_binary_search_tree** tree                = NULL;


    // NULL checks
    if( Tree == NULL || Key == NULL || Tree_At_Key == NULL || Compare_Symbol == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message( cpfphig_system_error, "Tree, Key, Tree_At_Key or Compare_Symbol is NULL", Error );

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
            *Tree_At_Key = Tree;

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
            ret = cpfphig_binary_search_tree_find_tree( *tree,
                                                        Key,
                                                        Tree_At_Key,
                                                        Compare_Symbol,
                                                        Error );
        }
    }

    return ret;
}



