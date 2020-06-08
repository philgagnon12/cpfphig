#include "cpfphig/cpfphig.h"
#include "cpfphig/binary_search_tree.h"

#include "cpfphig/malloc.h"
#include "cpfphig/free.h"

cpfphig
CPFPHIG_REAL(cpfphig_binary_search_tree_insert)( struct cpfphig_binary_search_tree*         Tree,
                                                 void*                                      Key,
                                                 void*                                      Item,
                                                 cpfphig_binary_search_tree_compare_symbol* Compare_Symbol,
                                                 CPFPHIG_OPTIONAL struct cpfphig_error*     Error )
{
    cpfphig ret                         = CPFPHIG_FAIL;
    int     compare                     = 0;

    struct cpfphig_binary_search_tree** tree                = NULL;
    struct cpfphig_binary_search_tree*  allocated_tree      = NULL;


    static const struct cpfphig_binary_search_tree      const_binary_search_tree  = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;

    // NULL checks
    if( Tree == NULL || Key == NULL || Item == NULL || Compare_Symbol == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message( cpfphig_system_error, "Tree, Key, Item or Compare_Symbol is NULL", Error );

        return CPFPHIG_FAIL;
    }


    if( Tree->parent == NULL &&
        Tree->left   == NULL &&
        Tree->right  == NULL &&
        Tree->key    == NULL &&
        Tree->item   == NULL )
    {
        Tree->key   = Key;
        Tree->item  = Item;
        return CPFPHIG_OK;
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
            if( Error != NULL )
                cpfphig_error_message( cpfphig_user_error, "Key already set", Error );

            ret = CPFPHIG_FAIL;
        }
    }

    // Got a node, but may be NULL, allocate when needed
    if( ret == CPFPHIG_OK )
    {
        if( *tree == NULL )
        {
            ret = cpfphig_malloc( sizeof( struct cpfphig_binary_search_tree ),
                                  &allocated_tree,
                                  Error );

            if( ret == CPFPHIG_OK )
            {
                *allocated_tree         = const_binary_search_tree;

                allocated_tree->parent  = Tree;
                allocated_tree->key     = Key;
                allocated_tree->item    = Item;

                *tree                   = allocated_tree;
            }
        }
        else
        {
            ret = cpfphig_binary_search_tree_insert( *tree,
                                                     Key,
                                                     Item,
                                                     Compare_Symbol,
                                                     Error );
        }
    }

    if( ret == CPFPHIG_FAIL )
    {
        cpfphig_free( &allocated_tree, NULL );
    }

    return ret;
}



