#include "cpfphig/cpfphig.h"
#include "cpfphig/binary_search_tree.h"
#include "cpfphig/free.h"

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

static void argument_tree_dest_null( void** state )
{
    struct cpfphig_binary_search_tree   tree_dest = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;
    struct cpfphig_binary_search_tree   tree_src  = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;

    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;

    expect_value( cpfphig_error_message_call, Error_Type, cpfphig_system_error );
    expect_string( cpfphig_error_message_call, Format, "Tree_Dest, Tree_Src or Compare_Symbol is NULL");

    assert_true( CPFPHIG_FAIL == cpfphig_binary_search_tree_merge( NULL,
                                                                   &tree_src,
                                                                   &cpfphig_binary_search_tree_compare_number,
                                                                   &error ) );
}

static void argument_tree_src_null( void** state )
{
    struct cpfphig_binary_search_tree   tree_dest = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;
    struct cpfphig_binary_search_tree   tree_src  = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;

    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;

    expect_value( cpfphig_error_message_call, Error_Type, cpfphig_system_error );
    expect_string( cpfphig_error_message_call, Format, "Tree_Dest, Tree_Src or Compare_Symbol is NULL");

    assert_true( CPFPHIG_FAIL == cpfphig_binary_search_tree_merge( &tree_dest,
                                                                   NULL,
                                                                   &cpfphig_binary_search_tree_compare_number,
                                                                   &error ) );
}

static void argument_compare_symbol_null( void** state )
{
    struct cpfphig_binary_search_tree   tree_dest = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;
    struct cpfphig_binary_search_tree   tree_src  = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;

    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;

    expect_value( cpfphig_error_message_call, Error_Type, cpfphig_system_error );
    expect_string( cpfphig_error_message_call, Format, "Tree_Dest, Tree_Src or Compare_Symbol is NULL");

    assert_true( CPFPHIG_FAIL == cpfphig_binary_search_tree_merge( &tree_dest,
                                                                   &tree_src,
                                                                   NULL,
                                                                   &error ) );
}

static void merge_empty_dest( void** state )
{
    struct cpfphig_binary_search_tree   tree_dest = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;
    struct cpfphig_binary_search_tree   tree_src  = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;

    int    key_a    = 50;
    char* item_a    = "50";


    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;

    will_return( cpfphig_binary_search_tree_insert, CPFPHIG_OK );
    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_insert( &tree_src,
                                                                  &key_a,
                                                                  item_a,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  NULL,
                                                                  &error ) );


    will_return( cpfphig_binary_search_tree_insert, CPFPHIG_OK );
    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_merge( &tree_dest,
                                                                 &tree_src,
                                                                 &cpfphig_binary_search_tree_compare_number,
                                                                 &error ) );

    assert_int_equal( key_a, *(int*)tree_dest.key );
    assert_string_equal( item_a, (char*)tree_dest.item );
    assert_null( tree_dest.left );
    assert_null( tree_dest.parent );
    assert_null( tree_dest.right );

}

static void merge_one_level( void** state )
{
    struct cpfphig_binary_search_tree   tree_dest = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;
    struct cpfphig_binary_search_tree   tree_src  = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;

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

    // Setup tree_dest

    will_return( cpfphig_binary_search_tree_insert, CPFPHIG_OK );
    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_insert( &tree_dest,
                                                                  &key_a,
                                                                  item_a,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  NULL,
                                                                  &error ) );

    expect_value( cpfphig_malloc, Size, sizeof( struct cpfphig_binary_search_tree ) );
    will_return( cpfphig_malloc, CPFPHIG_OK );

    will_return( cpfphig_binary_search_tree_insert, CPFPHIG_OK );
    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_insert( &tree_dest,
                                                                  &key_b,
                                                                  item_b,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  NULL,
                                                                  &error ) );


    expect_value( cpfphig_malloc, Size, sizeof( struct cpfphig_binary_search_tree ) );
    will_return( cpfphig_malloc, CPFPHIG_OK );

    will_return( cpfphig_binary_search_tree_insert, CPFPHIG_OK );
    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_insert( &tree_dest,
                                                                  &key_c,
                                                                  item_c,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  NULL,
                                                                  &error ) );


    will_return( cpfphig_binary_search_tree_insert, CPFPHIG_OK );
    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_insert( &tree_src,
                                                                  &key_d,
                                                                  item_d,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  NULL,
                                                                  &error ) );

    expect_value( cpfphig_malloc, Size, sizeof( struct cpfphig_binary_search_tree ) );
    will_return( cpfphig_malloc, CPFPHIG_OK );

    will_return( cpfphig_binary_search_tree_insert, CPFPHIG_OK );
    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_insert( &tree_src,
                                                                  &key_e,
                                                                  item_e,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  NULL,
                                                                  &error ) );


    expect_value( cpfphig_malloc, Size, sizeof( struct cpfphig_binary_search_tree ) );
    will_return( cpfphig_malloc, CPFPHIG_OK );

    will_return( cpfphig_binary_search_tree_insert, CPFPHIG_OK );
    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_insert( &tree_src,
                                                                  &key_f,
                                                                  item_f,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  NULL,
                                                                  &error ) );

    expect_value_count( cpfphig_malloc, Size, sizeof( struct cpfphig_binary_search_tree ), 3);
    will_return_count( cpfphig_malloc, CPFPHIG_OK, 3 );
    will_return_count( cpfphig_binary_search_tree_insert, CPFPHIG_OK, 6 );
    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_merge( &tree_dest,
                                                                 &tree_src,
                                                                 &cpfphig_binary_search_tree_compare_number,
                                                                 &error ) );

    assert_int_equal( key_a, *(int*)tree_dest.key );

    assert_non_null( tree_dest.left );
    assert_non_null( tree_dest.right );
    assert_int_equal( key_b, *(int*)tree_dest.left->key );
    assert_int_equal( key_c, *(int*)tree_dest.right->key );

    assert_non_null( tree_dest.left->left );
    assert_null( tree_dest.left->right );
    assert_int_equal( key_e, *(int*)tree_dest.left->left->key );

    assert_non_null( tree_dest.right->left );
    assert_non_null( tree_dest.right->right );
    assert_int_equal( key_d, *(int*)tree_dest.right->left->key );
    assert_int_equal( key_f, *(int*)tree_dest.right->right->key );


    expect_any_count( cpfphig_free, Ptr, 7 );
    will_return_count( cpfphig_free, CPFPHIG_OK, 7 );
    assert_true( CPFPHIG_OK == cpfphig_free( &tree_dest.left->left, NULL ) );
    assert_true( CPFPHIG_OK == cpfphig_free( &tree_dest.left, NULL ) );

    assert_true( CPFPHIG_OK == cpfphig_free( &tree_dest.right->left, NULL ) );
    assert_true( CPFPHIG_OK == cpfphig_free( &tree_dest.right->right, NULL ) );
    assert_true( CPFPHIG_OK == cpfphig_free( &tree_dest.right, NULL ) );

    assert_true( CPFPHIG_OK == cpfphig_free( &tree_src.left, NULL ) );
    assert_true( CPFPHIG_OK == cpfphig_free( &tree_src.right, NULL ) );

}

int main( void )

{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(argument_tree_dest_null),
        cmocka_unit_test(argument_tree_src_null),
        cmocka_unit_test(argument_compare_symbol_null),

        cmocka_unit_test(merge_empty_dest),
        cmocka_unit_test(merge_one_level),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
