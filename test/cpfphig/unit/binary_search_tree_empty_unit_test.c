#include "cpfphig/cpfphig.h"
#include "cpfphig/binary_search_tree.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <string.h>


static
cpfphig
cpfphig_binary_search_tree_compare_number( void*                     key,
                                           void*                     tree_key,
                                           int*                      result,
                                           CPFPHIG_OPTIONAL struct   cpfphig_error* Error )
{
    int* int_key        = (int*)key;
    int* int_tree_key   = (int*)tree_key;

    if( *int_key < *int_tree_key )
    {
        *result = -1;
    }
    else if( *int_key > *int_tree_key )
    {
        *result = 1;
    }
    else
    {
        *result = 0;
    }

    return CPFPHIG_OK;
}

static void argument_tree_null( void** state )
{
    struct cpfphig_binary_search_tree   tree = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;

    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;

    expect_value( cpfphig_error_message_call, Error_Type, cpfphig_system_error );
    expect_string( cpfphig_error_message_call, Format, "Tree is NULL");

    assert_true( CPFPHIG_FAIL == cpfphig_binary_search_tree_empty( NULL,
                                                                   &error ) );
}



static void empty( void** state )
{
    struct cpfphig_binary_search_tree   tree = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;

    int    key_a    = 50;
    char* item_a    = "50";

    int    key_b    = 45;
    char* item_b    = "45";

    int    key_c    = 55;
    char* item_c    = "55";

    int    key_d    = 52;
    char* item_d    = "52";

    int    key_e    = 42;
    char* item_e    = "42";

    int    key_f    = 57;
    char* item_f    = "57";



    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;

    // Setup tree

    will_return( cpfphig_binary_search_tree_insert, CPFPHIG_OK );
    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_insert( &tree,
                                                                  &key_a,
                                                                  item_a,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  NULL,
                                                                  &error ) );

    expect_value( cpfphig_malloc, Size, sizeof( struct cpfphig_binary_search_tree ) );
    will_return( cpfphig_malloc, CPFPHIG_OK );

    will_return( cpfphig_binary_search_tree_insert, CPFPHIG_OK );
    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_insert( &tree,
                                                                  &key_b,
                                                                  item_b,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  NULL,
                                                                  &error ) );


    expect_value( cpfphig_malloc, Size, sizeof( struct cpfphig_binary_search_tree ) );
    will_return( cpfphig_malloc, CPFPHIG_OK );

    will_return( cpfphig_binary_search_tree_insert, CPFPHIG_OK );
    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_insert( &tree,
                                                                  &key_c,
                                                                  item_c,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  NULL,
                                                                  &error ) );

    expect_value( cpfphig_malloc, Size, sizeof( struct cpfphig_binary_search_tree ) );
    will_return( cpfphig_malloc, CPFPHIG_OK );

    will_return_count( cpfphig_binary_search_tree_insert, CPFPHIG_OK, 2);
    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_insert( &tree,
                                                                  &key_d,
                                                                  item_d,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  NULL,
                                                                  &error ) );

    expect_value( cpfphig_malloc, Size, sizeof( struct cpfphig_binary_search_tree ) );
    will_return( cpfphig_malloc, CPFPHIG_OK );

    will_return_count( cpfphig_binary_search_tree_insert, CPFPHIG_OK, 2);
    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_insert( &tree,
                                                                  &key_e,
                                                                  item_e,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  NULL,
                                                                  &error ) );


    expect_value( cpfphig_malloc, Size, sizeof( struct cpfphig_binary_search_tree ) );
    will_return( cpfphig_malloc, CPFPHIG_OK );

    will_return_count( cpfphig_binary_search_tree_insert, CPFPHIG_OK, 2);
    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_insert( &tree,
                                                                  &key_f,
                                                                  item_f,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  NULL,
                                                                  &error ) );


    expect_any_count( cpfphig_free, Ptr, 5 );
    will_return_count( cpfphig_free, CPFPHIG_OK, 5 );

    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_empty( &tree,
                                                                 &error ) );

    assert_null( tree.key );
    assert_null( tree.item );
    assert_null( tree.left );
    assert_null( tree.right );
    assert_null( tree.parent );
}

int main( void )

{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(argument_tree_null),
        cmocka_unit_test(empty),

    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
