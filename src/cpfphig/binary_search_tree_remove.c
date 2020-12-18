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
    struct cpfphig_binary_search_tree*      parent_or_root = NULL;

    struct cpfphig_binary_search_tree*      trees[] = { NULL, NULL };
    struct cpfphig_binary_search_tree*      tree    = NULL;

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

        trees[0] = tree_at_key->left;
        trees[1] = tree_at_key->right;
        // Assign as parent
        


        if( tree_at_key->parent == NULL )
        {
            if( tree_at_key == Tree )
            {
                // Remove on root is simple
                Tree->item = NULL;
                Tree->key = NULL;
                Tree->left = NULL;
                Tree->right = NULL;

                // Assign as root
                parent_or_root = Tree;
            }
            else
            {
                if( Error != NULL )
                    cpfphig_error_message( cpfphig_system_error, "tree_at_key is not Tree", Error );

                ret = CPFPHIG_FAIL;
            }
        }
        else
        {
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

            if( ret == CPFPHIG_OK )
            {
                parent_or_root = tree_at_key->parent;
            }
        }
    }

    for( int i = 0;
         ret == CPFPHIG_OK &&
         i < sizeof( trees ) / sizeof( struct cpfphig_binary_search_tree*);
         i++ )
    {
        tree = trees[i];

        if( tree != NULL )
        {
            ret = cpfphig_binary_search_tree_merge( parent_or_root,
                                                    tree,
                                                    Compare_Symbol,
                                                    Error );

            if( ret == CPFPHIG_OK )
            {
                ret = cpfphig_binary_search_tree_empty( tree,
                                                        Error );
            }

            if( ret == CPFPHIG_OK )
            {
                ret = cpfphig_free( &tree,
                                    Error );
            }
        }
    }


    if( ret == CPFPHIG_OK )
    {
        if( tree_at_key != Tree )
        {
            ret = cpfphig_free( &tree_at_key,
                                Error );
        }
    }


    return ret;
}



