#include "cpfphig/cpfphig.h"
#include "cpfphig/binary_search_tree.h"
#include "cpfphig/unit/mock/binary_search_tree_mock.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

cpfphig
cpfphig_binary_search_tree_insert( struct cpfphig_binary_search_tree*         Tree,
                                   void*                                      Key,
                                   void*                                      Item,
                                   cpfphig_binary_search_tree_compare_symbol* Compare_Symbol,
                                   CPFPHIG_OPTIONAL struct cpfphig_error*     Error )
{
#ifndef CPFPHIG_BINARY_SEARCH_TREE_RECURSIVE_MOCK
    assert_non_null( Tree );
    assert_non_null( Key );
    assert_non_null( Item );
    assert_non_null( Compare_Symbol );
    // Call the real function
    cpfphig ret = (cpfphig)mock();

    if( ret == CPFPHIG_OK )
    {
        // Expecting the real thing to work but if it doesnt, have to make sure CPFPHIG_FAIL is returned
        ret = real_cpfphig_binary_search_tree_insert( Tree,
                                                      Key,
                                                      Item,
                                                      Compare_Symbol,
                                                      Error );
    }

    return ret;
#else

    return real_cpfphig_binary_search_tree_insert( Tree,
                                                   Key,
                                                   Item,
                                                   Compare_Symbol,
                                                   Error );
#endif
}
