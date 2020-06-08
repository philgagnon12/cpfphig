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

static void argument_tree_null( void** state )
{
    struct cpfphig_binary_search_tree   tree = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;

    int key         = 50;
    char* item_a    = "50";

    struct cpfphig_binary_search_tree*   tree_at_key = NULL;

    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;

    expect_value( cpfphig_error_message_call, Error_Type, cpfphig_system_error );
    expect_string( cpfphig_error_message_call, Format, "Tree, Key, Item or Compare_Symbol is NULL");

    assert_true( CPFPHIG_FAIL == cpfphig_binary_search_tree_insert( NULL,
                                                                    &key,
                                                                    item_a,
                                                                    &cpfphig_binary_search_tree_compare_number,
                                                                    NULL,
                                                                    &error ) );
}

static void argument_key_null( void** state )
{
    struct cpfphig_binary_search_tree   tree = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;

    int key         = 50;
    char* item_a    = "50";

    struct cpfphig_binary_search_tree*   tree_at_key = NULL;

    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;


    expect_value( cpfphig_error_message_call, Error_Type, cpfphig_system_error );
    expect_string( cpfphig_error_message_call, Format, "Tree, Key, Item or Compare_Symbol is NULL");

    assert_true( CPFPHIG_FAIL == cpfphig_binary_search_tree_insert( &tree,
                                                                    NULL,
                                                                    item_a,
                                                                    &cpfphig_binary_search_tree_compare_number,
                                                                    NULL,
                                                                    &error ) );
}

static void argument_item_null( void** state )
{
    struct cpfphig_binary_search_tree   tree = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;

    int key         = 50;
    char* item_a    = "50";

    struct cpfphig_binary_search_tree*   tree_at_key = NULL;

    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;


    expect_value( cpfphig_error_message_call, Error_Type, cpfphig_system_error );
    expect_string( cpfphig_error_message_call, Format, "Tree, Key, Item or Compare_Symbol is NULL");

    assert_true( CPFPHIG_FAIL == cpfphig_binary_search_tree_insert( &tree,
                                                                    &key,
                                                                    NULL,
                                                                    &cpfphig_binary_search_tree_compare_number,
                                                                    NULL,
                                                                    &error ) );
}

static void argument_compare_symbol_null( void** state )
{
    struct cpfphig_binary_search_tree   tree = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;

    int key         = 50;
    char* item_a    = "50";

    struct cpfphig_binary_search_tree*   tree_at_key = NULL;

    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;


    expect_value( cpfphig_error_message_call, Error_Type, cpfphig_system_error );
    expect_string( cpfphig_error_message_call, Format, "Tree, Key, Item or Compare_Symbol is NULL");

    assert_true( CPFPHIG_FAIL == cpfphig_binary_search_tree_insert( &tree,
                                                                    &key,
                                                                    item_a,
                                                                    NULL,
                                                                    NULL,
                                                                    &error ) );
}

static void insert_root( void** state )
{
    struct cpfphig_binary_search_tree   tree = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;

    int key         = 50;
    char* item_a    = "50";

    struct cpfphig_binary_search_tree*   tree_at_key = NULL;

    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;



    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_insert( &tree,
                                                                  &key,
                                                                  item_a,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  &tree_at_key,
                                                                  &error ) );

    assert_int_equal( key, *(int*)tree.key );
    assert_string_equal( item_a, (char*)tree.item );
    assert_null( tree.left );
    assert_null( tree.parent );
    assert_null( tree.right );

    assert_true( &tree == tree_at_key );
}

static void insert_root_duplicate( void** state )
{
    struct cpfphig_binary_search_tree   tree = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;

    int key         = 50;
    char* item_a    = "50";

    struct cpfphig_binary_search_tree*   tree_at_key = NULL;

    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;



    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_insert( &tree,
                                                                  &key,
                                                                  item_a,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  NULL,
                                                                  &error ) );


    expect_value( cpfphig_error_message_call, Error_Type, cpfphig_user_error );
    expect_string( cpfphig_error_message_call, Format, "Key already set");

    expect_value( cpfphig_free, Ptr, NULL );
    will_return( cpfphig_free, CPFPHIG_FAIL );

    assert_true( CPFPHIG_FAIL == cpfphig_binary_search_tree_insert( &tree,
                                                                    &key,
                                                                    item_a,
                                                                    &cpfphig_binary_search_tree_compare_number,
                                                                    NULL,
                                                                    &error ) );
}

static void insert_left( void** state )
{
    struct cpfphig_binary_search_tree   tree = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;

    int    key_a    = 50;
    char* item_a    = "50";

    int    key_b    = 45;
    char* item_b    = "45";

    struct cpfphig_binary_search_tree*   tree_at_key = NULL;

    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;



    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_insert( &tree,
                                                                  &key_a,
                                                                  item_a,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  NULL,
                                                                  &error ) );

    assert_int_equal( key_a, *(int*)tree.key );
    assert_string_equal( item_a, (char*)tree.item );
    assert_null( tree.left );
    assert_null( tree.parent );
    assert_null( tree.right );

    expect_value( cpfphig_malloc, Size, sizeof( struct cpfphig_binary_search_tree ) );
    will_return( cpfphig_malloc, CPFPHIG_OK );

    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_insert( &tree,
                                                                  &key_b,
                                                                  item_b,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  NULL,
                                                                  &error ) );

    assert_non_null( tree.left );
    assert_null( tree.right );
    assert_int_equal( key_b, *(int*)tree.left->key );
    assert_string_equal( item_b, (char*)tree.left->item );
    assert_null( tree.left->left );
    assert_non_null( tree.left->parent );
    assert_null( tree.left->right );



    expect_any( cpfphig_free, Ptr );
    will_return( cpfphig_free, CPFPHIG_OK );
    assert_true( CPFPHIG_OK == cpfphig_free( &tree.left, NULL ) );
}

static void insert_left_left_right( void** state )
{
    struct cpfphig_binary_search_tree   tree = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;

    int    key_a    = 50;
    char* item_a    = "50";

    int    key_b    = 45;
    char* item_b    = "45";

    int    key_c    = 39;
    char* item_c    = "39";

    int    key_d    = 42;
    char* item_d    = "42";


    struct cpfphig_binary_search_tree*   tree_at_key = NULL;

    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;



    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_insert( &tree,
                                                                  &key_a,
                                                                  item_a,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  NULL,
                                                                  &error ) );

    assert_int_equal( key_a, *(int*)tree.key );
    assert_string_equal( item_a, (char*)tree.item );
    assert_null( tree.left );
    assert_null( tree.parent );
    assert_null( tree.right );


    expect_value( cpfphig_malloc, Size, sizeof( struct cpfphig_binary_search_tree ) );
    will_return( cpfphig_malloc, CPFPHIG_OK );

    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_insert( &tree,
                                                                  &key_b,
                                                                  item_b,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  NULL,
                                                                  &error ) );

    assert_non_null( tree.left );
    assert_null( tree.right );
    assert_int_equal(     key_b, *(int*)tree.left->key );
    assert_string_equal( item_b, (char*)tree.left->item );
    assert_null( tree.left->left );
    assert_non_null( tree.left->parent );
    assert_null( tree.left->right );



    expect_value( cpfphig_malloc, Size, sizeof( struct cpfphig_binary_search_tree ) );
    will_return( cpfphig_malloc, CPFPHIG_OK );

    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_insert( &tree,
                                                                  &key_c,
                                                                  item_c,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  NULL,
                                                                  &error ) );

    assert_non_null( tree.left->left );
    assert_null( tree.right );
    assert_int_equal(     key_c, *(int*)tree.left->left->key );
    assert_string_equal( item_c, (char*)tree.left->left->item );
    assert_null( tree.left->left->left );
    assert_non_null( tree.left->left->parent );
    assert_null( tree.left->left->right );


    expect_value( cpfphig_malloc, Size, sizeof( struct cpfphig_binary_search_tree ) );
    will_return( cpfphig_malloc, CPFPHIG_OK );

    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_insert( &tree,
                                                                  &key_d,
                                                                  item_d,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  NULL,
                                                                  &error ) );


    assert_non_null( tree.left->left->right );
    assert_int_equal(     key_d, *(int*)tree.left->left->right->key );
    assert_string_equal( item_d, (char*)tree.left->left->right->item );
    assert_null( tree.left->left->right->left );
    assert_non_null( tree.left->left->right->parent );
    assert_null( tree.left->left->right->right );



    expect_any_count( cpfphig_free, Ptr, 3 );
    will_return_count( cpfphig_free, CPFPHIG_OK, 3 );
    assert_true( CPFPHIG_OK == cpfphig_free( &tree.left->left->right, NULL ) );
    assert_true( CPFPHIG_OK == cpfphig_free( &tree.left->left, NULL ) );
    assert_true( CPFPHIG_OK == cpfphig_free( &tree.left, NULL ) );
}

int main( void )

{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(argument_tree_null),
        cmocka_unit_test(argument_key_null),
        cmocka_unit_test(argument_item_null),
        cmocka_unit_test(argument_compare_symbol_null),

        cmocka_unit_test(insert_root),
        cmocka_unit_test(insert_root_duplicate),

        cmocka_unit_test(insert_left),
        cmocka_unit_test(insert_left_left_right),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
