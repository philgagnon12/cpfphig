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

    struct cpfphig_binary_search_tree*      tree_at_key = NULL;


    // NULL checks
    if( Tree == NULL || Key == NULL || Item == NULL || Compare_Symbol == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message( cpfphig_system_error, "Tree, Key, Item or Compare_Symbol is NULL", Error );

        return CPFPHIG_FAIL;
    }

    ret = cpfphig_binary_search_tree_find_tree( Tree,
                                                Key,
                                                &tree_at_key,
                                                Compare_Symbol,
                                                Error );

    if( ret == CPFPHIG_OK )
    {
        *Item = tree_at_key->item;
    }

    return ret;
}



