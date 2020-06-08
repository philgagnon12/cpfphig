#include "cpfphig/cpfphig.h"
#include "cpfphig/binary_search_tree.h"

cpfphig
cpfphig_binary_search_tree_merge( struct cpfphig_binary_search_tree*            Tree_Dest,
                                  struct cpfphig_binary_search_tree*            Tree_Src,
                                  cpfphig_binary_search_tree_compare_symbol*    Compare_Symbol,
                                  CPFPHIG_OPTIONAL struct cpfphig_error*        Error )
{
    cpfphig ret                         = CPFPHIG_FAIL;
    int     compare                     = 0;

    struct cpfphig_binary_search_tree*      tree    = NULL;
    struct cpfphig_binary_search_tree*      trees[] = { NULL, NULL };


    // NULL checks
    if( Tree_Dest == NULL || Tree_Src == NULL || Compare_Symbol == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message( cpfphig_system_error, "Tree_Dest, Tree_Src or Compare_Symbol is NULL", Error );

        return CPFPHIG_FAIL;
    }


    // Insert root
    ret = cpfphig_binary_search_tree_insert( Tree_Dest,
                                             Tree_Src->key,
                                             Tree_Src->item,
                                             Compare_Symbol,
                                             NULL,
                                             Error );

    if( ret == CPFPHIG_OK )
    {
        trees[0] = Tree_Src->left;
        trees[1] = Tree_Src->right;
    }

    for( int i = 0;
         ret == CPFPHIG_OK &&
         i < sizeof( trees ) / sizeof( struct cpfphig_binary_search_tree*);
         i++ )
    {
        tree = trees[i];

        if( tree != NULL )
        {

            ret = cpfphig_binary_search_tree_merge( Tree_Dest,
                                                    tree,
                                                    Compare_Symbol,
                                                    Error );
        }
    }

    return ret;
}



