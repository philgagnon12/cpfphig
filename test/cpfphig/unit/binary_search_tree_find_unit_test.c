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

    char* found = NULL;

    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;

    expect_value( cpfphig_error_message_call, Error_Type, cpfphig_system_error );
    expect_string( cpfphig_error_message_call, Format, "Tree, Key, Item or Compare_Symbol is NULL");

    assert_true( CPFPHIG_FAIL == cpfphig_binary_search_tree_find( NULL,
                                                                  &key,
                                                                  (void**)&found,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  &error ) );
}

static void argument_key_null( void** state )
{
    struct cpfphig_binary_search_tree   tree = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;

    int key         = 50;
    char* item_a    = "50";

    char* found = NULL;

    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;


    expect_value( cpfphig_error_message_call, Error_Type, cpfphig_system_error );
    expect_string( cpfphig_error_message_call, Format, "Tree, Key, Item or Compare_Symbol is NULL");

    assert_true( CPFPHIG_FAIL == cpfphig_binary_search_tree_find( &tree,
                                                                  NULL,
                                                                  (void**)&found,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  &error ) );
}

static void argument_item_null( void** state )
{
    struct cpfphig_binary_search_tree   tree = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;

    int key         = 50;
    char* item_a    = "50";

    char* found = NULL;

    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;


    expect_value( cpfphig_error_message_call, Error_Type, cpfphig_system_error );
    expect_string( cpfphig_error_message_call, Format, "Tree, Key, Item or Compare_Symbol is NULL");

    assert_true( CPFPHIG_FAIL == cpfphig_binary_search_tree_find( &tree,
                                                                  &key,
                                                                  NULL,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  &error ) );
}

static void argument_compare_symbol_null( void** state )
{
    struct cpfphig_binary_search_tree   tree = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;

    int key         = 50;
    char* item_a    = "50";

    char* found = NULL;

    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;


    expect_value( cpfphig_error_message_call, Error_Type, cpfphig_system_error );
    expect_string( cpfphig_error_message_call, Format, "Tree, Key, Item or Compare_Symbol is NULL");

    assert_true( CPFPHIG_FAIL == cpfphig_binary_search_tree_find( &tree,
                                                                  &key,
                                                                  (void**)&found,
                                                                  NULL,
                                                                  &error ) );
}

static void find_empty_tree(void** state )
{
    struct cpfphig_binary_search_tree   tree = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;

    int key         = 50;
    char* item_a    = "50";

    char* found = NULL;

    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;


    expect_value( cpfphig_error_message_call, Error_Type, cpfphig_system_error );
    expect_string( cpfphig_error_message_call, Format, "Tree->key is NULL");

    assert_true( CPFPHIG_FAIL == cpfphig_binary_search_tree_find( &tree,
                                                                  &key,
                                                                  (void**)&found,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  &error ) );
}

static void find_root( void** state )
{
    struct cpfphig_binary_search_tree   tree = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;

    int    key_a    = 50;
    char* item_a    = "50";

    char* found = NULL;

    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;


    will_return( cpfphig_binary_search_tree_insert, CPFPHIG_OK );
    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_insert( &tree,
                                                                  &key_a,
                                                                  item_a,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  &error ) );

    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_find( &tree,
                                                                &key_a,
                                                                (void**)&found,
                                                                &cpfphig_binary_search_tree_compare_number,
                                                                &error ) );

    assert_string_equal( item_a, found );
}

static void find_root_not_found( void** state )
{
    struct cpfphig_binary_search_tree   tree = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;

    int    key_a    = 50;
    char* item_a    = "50";

    int    key_b    = 60;

    char* found = NULL;

    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;


    will_return( cpfphig_binary_search_tree_insert, CPFPHIG_OK );
    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_insert( &tree,
                                                                  &key_a,
                                                                  item_a,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  &error ) );

    expect_value( cpfphig_error_message_call, Error_Type, cpfphig_user_error );
    expect_string( cpfphig_error_message_call, Format, "Key not found");

    assert_true( CPFPHIG_FAIL == cpfphig_binary_search_tree_find( &tree,
                                                                  &key_b,
                                                                  (void**)&found,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  &error ) );

}

static void find_left( void** state )
{
    struct cpfphig_binary_search_tree   tree = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;

    int    key_a    = 50;
    char* item_a    = "50";

    int    key_b    = 45;
    char* item_b    = "45";

    char* found = NULL;

    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;


    will_return( cpfphig_binary_search_tree_insert, CPFPHIG_OK );
    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_insert( &tree,
                                                                  &key_a,
                                                                  item_a,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  &error ) );


    expect_value( cpfphig_malloc, Size, sizeof( struct cpfphig_binary_search_tree ) );
    will_return( cpfphig_malloc, CPFPHIG_OK );

    will_return( cpfphig_binary_search_tree_insert, CPFPHIG_OK );
    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_insert( &tree,
                                                                  &key_b,
                                                                  item_b,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  &error ) );

    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_find( &tree,
                                                                &key_b,
                                                                (void**)&found,
                                                                &cpfphig_binary_search_tree_compare_number,
                                                                &error ) );

    assert_string_equal( item_b, found );

    expect_any( cpfphig_free, Ptr );
    will_return( cpfphig_free, CPFPHIG_OK );
    assert_true( CPFPHIG_OK == cpfphig_free( &tree.left, NULL ) );
}

static void find_right( void** state )
{
    struct cpfphig_binary_search_tree   tree = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;

    int    key_a    = 50;
    char* item_a    = "50";

    int    key_b    = 60;
    char* item_b    = "60";

    char* found = NULL;

    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;


    will_return( cpfphig_binary_search_tree_insert, CPFPHIG_OK );
    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_insert( &tree,
                                                                  &key_a,
                                                                  item_a,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  &error ) );


    expect_value( cpfphig_malloc, Size, sizeof( struct cpfphig_binary_search_tree ) );
    will_return( cpfphig_malloc, CPFPHIG_OK );

    will_return( cpfphig_binary_search_tree_insert, CPFPHIG_OK );
    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_insert( &tree,
                                                                  &key_b,
                                                                  item_b,
                                                                  &cpfphig_binary_search_tree_compare_number,
                                                                  &error ) );

    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_find( &tree,
                                                                &key_b,
                                                                (void**)&found,
                                                                &cpfphig_binary_search_tree_compare_number,
                                                                &error ) );

    assert_string_equal( item_b, found );

    expect_any( cpfphig_free, Ptr );
    will_return( cpfphig_free, CPFPHIG_OK );
    assert_true( CPFPHIG_OK == cpfphig_free( &tree.right, NULL ) );
}

int main( void )

{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(argument_tree_null),
        cmocka_unit_test(argument_key_null),
        cmocka_unit_test(argument_item_null),
        cmocka_unit_test(argument_compare_symbol_null),

        cmocka_unit_test(find_empty_tree),
        cmocka_unit_test(find_root),
        cmocka_unit_test(find_root_not_found),

        cmocka_unit_test(find_left),
        cmocka_unit_test(find_right),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
