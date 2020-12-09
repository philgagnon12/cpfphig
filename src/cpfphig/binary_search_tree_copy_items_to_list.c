#include "cpfphig/cpfphig.h"
#include "cpfphig/binary_search_tree.h"

#include "cpfphig/list.h"

cpfphig
cpfphig_binary_search_tree_copy_items_to_list( struct cpfphig_binary_search_tree*       Tree,
                                               struct cpfphig_list*                     Items,
                                               CPFPHIG_OPTIONAL struct cpfphig_error*   Error )
{
    cpfphig ret                         = CPFPHIG_FAIL;
    int     compare_min                 = 0;
    int     compare_max                 = 0;

    struct cpfphig_binary_search_tree**     tree    = NULL;
    struct cpfphig_binary_search_tree**     trees[] = { NULL, NULL };


    // NULL checks
    if( Tree == NULL ||  Items == NULL)
    {
        if( Error != NULL )
            cpfphig_error_message( cpfphig_system_error, "Tree or Items is NULL", Error );

        return CPFPHIG_FAIL;
    }

    trees[0] = &Tree->left;
    trees[1] = &Tree->right;

    if( Tree->item != NULL )
    {
        ret = cpfphig_list_push( Items,
                                 Tree->item,
                                 Error );
    }

    if( ret == CPFPHIG_OK )
    {
        if( trees[0] != NULL || trees[1] != NULL )
        {

            for( int i = 0;
                 ret == CPFPHIG_OK &&
                 i < sizeof( trees ) / sizeof( struct cpfphig_binary_search_tree** );
                 i++ )
            {
                tree = trees[i];

                if( tree != NULL )
                {
                    if( *tree != NULL )
                    {
                        ret = cpfphig_binary_search_tree_copy_items_to_list( *tree,
                                                                             Items,
                                                                             Error );
                    }
                }
            } // for
        }
    }

    if( Items->first == NULL )
    {
         if( Error != NULL )
            cpfphig_error_message( cpfphig_user_error, "Tree and Items are empty", Error );

         ret = CPFPHIG_FAIL;
    }

    return ret;
}



