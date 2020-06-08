#include "cpfphig/cpfphig.h"
#include "cpfphig/binary_search_tree.h"

#include "cpfphig/free.h"

cpfphig
cpfphig_binary_search_tree_remove( struct cpfphig_binary_search_tree*         Tree,
                                   void*                                      Key,
                                   cpfphig_binary_search_tree_compare_symbol* Compare_Symbol,
                                   CPFPHIG_OPTIONAL void**                    Item,
                                   CPFPHIG_OPTIONAL struct cpfphig_error*     Error )
{
    cpfphig ret                         = CPFPHIG_FAIL;

    struct cpfphig_binary_search_tree*      tree_at_key = NULL;


    // NULL checks
    if( Tree == NULL || Key == NULL || Compare_Symbol == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message( cpfphig_system_error, "Tree, Key or Compare_Symbol is NULL", Error );

        return CPFPHIG_FAIL;
    }


    ret = cpfphig_binary_search_tree_find_tree( Tree,
                                                Key,
                                                &tree_at_key,
                                                Compare_Symbol,
                                                Error );

    if( ret == CPFPHIG_OK )
    {
        if( Item != NULL )
        {
            *Item = tree_at_key->item;
        }

        // Disconnect tree from parent

        if( tree_at_key->parent->left == tree_at_key )
        {
            tree_at_key->parent->left = NULL;
        }
        else if( tree_at_key->parent->right == tree_at_key )
        {
            tree_at_key->parent->right = NULL;
        }
        else
        {
            if( Error != NULL )
                cpfphig_error_message( cpfphig_system_error, "tree_at_key is not a child of tree_at_key->parent", Error );

            ret = CPFPHIG_FAIL;
        }
    }

    if( ret == CPFPHIG_OK )
    {
        if( tree_at_key->left != NULL )
        {
            ret = cpfphig_binary_search_tree_merge( tree_at_key->parent,
                                                    tree_at_key->left,
                                                    Compare_Symbol,
                                                    Error );

            if( ret == CPFPHIG_OK )
            {
                ret = cpfphig_binary_search_tree_empty( tree_at_key->left,
                                                        Error );
            }

            if( ret == CPFPHIG_OK )
            {
                ret = cpfphig_free( &tree_at_key->left,
                                    Error );
            }
        }
    }

    if( ret == CPFPHIG_OK )
    {
        if( tree_at_key->right != NULL )
        {
            ret = cpfphig_binary_search_tree_merge( tree_at_key->parent,
                                                    tree_at_key->right,
                                                    Compare_Symbol,
                                                    Error );

            if( ret == CPFPHIG_OK )
            {
                ret = cpfphig_binary_search_tree_empty( tree_at_key->right,
                                                        Error );
            }

            if( ret == CPFPHIG_OK )
            {
                ret = cpfphig_free( &tree_at_key->left,
                                    Error );
            }
        }
    }

    if( ret == CPFPHIG_OK )
    {
        ret = cpfphig_free( &tree_at_key,
                            Error );
    }


    return ret;
}



