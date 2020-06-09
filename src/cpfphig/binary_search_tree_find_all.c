#include "cpfphig/cpfphig.h"
#include "cpfphig/binary_search_tree.h"

#include "cpfphig/list.h"

cpfphig
cpfphig_binary_search_tree_find_all( struct cpfphig_binary_search_tree*           Tree,
                                     void*                                        Key_Min,
                                     void*                                        Key_Max,
                                     cpfphig_binary_search_tree_compare_symbol*   Compare_Symbol,
                                     struct cpfphig_list*                         Items,
                                     CPFPHIG_OPTIONAL struct cpfphig_error*       Error )
{
    cpfphig ret                         = CPFPHIG_FAIL;
    int     compare_min                 = 0;
    int     compare_max                 = 0;

    struct cpfphig_binary_search_tree**     tree    = NULL;
    struct cpfphig_binary_search_tree**     trees[] = { NULL, NULL };


    // NULL checks
    if( Tree == NULL || Key_Min == NULL || Key_Max == NULL || Items == NULL || Compare_Symbol == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message( cpfphig_system_error, "Tree, Key_Min, Key_Max, Items or Compare_Symbol is NULL", Error );

        return CPFPHIG_FAIL;
    }

    if( Tree->key == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message( cpfphig_system_error, "Tree->key is NULL", Error );

        return CPFPHIG_FAIL;
    }

    ret  = Compare_Symbol( Key_Min, Tree->key, &compare_min, Error );

    if( ret == CPFPHIG_OK )
    {
        ret  = Compare_Symbol( Key_Max, Tree->key, &compare_max, Error );
    }

    if( ret == CPFPHIG_OK )
    {
        if( compare_min < 0 || compare_min == 0 )
        {
            trees[0] = &Tree->left;
        }

        if ( compare_max > 0 || compare_max == 0 )
        {
            trees[1] = &Tree->right;
        }
    }

    if( ret == CPFPHIG_OK )
    {
        if( trees[0] != NULL && trees[1] != NULL )
        {

            ret = cpfphig_list_push( Items,
                                     Tree->item,
                                     Error );
        }
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
                        ret = cpfphig_binary_search_tree_find_all( *tree,
                                                                   Key_Min,
                                                                   Key_Max,
                                                                   Compare_Symbol,
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
            cpfphig_error_message( cpfphig_user_error, "No match found for Key", Error );

         ret = CPFPHIG_FAIL;
    }

    return ret;
}



