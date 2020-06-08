#include "cpfphig/cpfphig.h"
#include "cpfphig/binary_search_tree.h"

#include "cpfphig/free.h"

cpfphig
cpfphig_binary_search_tree_empty( struct cpfphig_binary_search_tree*            Tree,
                                  CPFPHIG_OPTIONAL struct cpfphig_error*        Error )
{
    cpfphig ret = CPFPHIG_FAIL;

    // NULL checks
    if( Tree == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message( cpfphig_system_error, "Tree is NULL", Error );

        return CPFPHIG_FAIL;
    }

    // Assume ok
    ret = CPFPHIG_OK;

    Tree->key       = NULL;
    Tree->item      = NULL;
    Tree->parent    = NULL;

    if( Tree->left != NULL )
    {
        ret = cpfphig_binary_search_tree_empty( Tree->left,
                                                Error );

        if( ret == CPFPHIG_OK )
        {
            ret = cpfphig_free( &Tree->left,
                                Error );
        }

        if( ret == CPFPHIG_OK )
        {
            Tree->left = NULL;
        }
    }

    if( ret == CPFPHIG_OK )
    {
        if( Tree->right != NULL )
        {
            ret = cpfphig_binary_search_tree_empty( Tree->right,
                                                    Error );

            if( ret == CPFPHIG_OK )
            {
                ret = cpfphig_free( &Tree->right,
                                    Error );
            }

            if( ret == CPFPHIG_OK )
            {
                Tree->right = NULL;
            }
        }
    }

    return ret;
}


