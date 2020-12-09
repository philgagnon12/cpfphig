#include "cpfphig/cpfphig.h"
#include "cpfphig/binary_search_tree.h"
#include "cpfphig/list.h"

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

    char* item = NULL;

    struct cpfphig_list items = CPFPHIG_CONST_CPFPHIG_LIST;

    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;

    expect_value( cpfphig_error_message_call, Error_Type, cpfphig_system_error );
    expect_string( cpfphig_error_message_call, Format, "Tree or Items is NULL");

    assert_true( CPFPHIG_FAIL == cpfphig_binary_search_tree_copy_items_to_list( NULL,
                                                                                &items,
                                                                                &error ) );
}

static void argument_items_null( void** state )
{
    struct cpfphig_binary_search_tree   tree = CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE;

    char* item = NULL;

    struct cpfphig_list items = CPFPHIG_CONST_CPFPHIG_LIST;

    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;

    expect_value( cpfphig_error_message_call, Error_Type, cpfphig_system_error );
    expect_string( cpfphig_error_message_call, Format, "Tree or Items is NULL");


    assert_true( CPFPHIG_FAIL == cpfphig_binary_search_tree_copy_items_to_list( &tree,
                                                                                NULL,
                                                                                &error ) );
}

static void copy_items_to_list( void** state )
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

    struct cpfphig_list items = CPFPHIG_CONST_CPFPHIG_LIST;

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


    expect_value_count( cpfphig_malloc, Size, sizeof( struct cpfphig_list_node ), 6 );
    will_return_count( cpfphig_malloc, CPFPHIG_OK, 6 );
    will_return_count( cpfphig_list_push, CPFPHIG_OK, 6);
    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_copy_items_to_list( &tree,
                                                                              &items,
                                                                              &error ) );

    assert_non_null( items.first );
    assert_string_equal( item_a, (char*)items.first->item );
    assert_non_null( items.first->next );
    assert_string_equal( item_b, (char*)items.first->next->item );
    assert_non_null( items.first->next->next );
    assert_string_equal( item_e, (char*)items.first->next->next->item );
    assert_non_null( items.first->next->next->next );
    assert_string_equal( item_c, (char*)items.first->next->next->next->item );
    assert_non_null( items.first->next->next->next->next );
    assert_string_equal( item_d, (char*)items.first->next->next->next->next->item );
    assert_non_null( items.first->next->next->next->next->next );
    assert_string_equal( item_f, (char*)items.first->next->next->next->next->next->item );

    assert_null( items.first->next->next->next->next->next->next );

    will_return_count( cpfphig_list_shift, CPFPHIG_OK, 7 );

    expect_any_count( cpfphig_free, Ptr, 6 );
    will_return_count( cpfphig_free, CPFPHIG_OK, 6 );

    expect_value( cpfphig_error_message_call, Error_Type, cpfphig_user_error );
    expect_string( cpfphig_error_message_call, Format, "List is empty");

    assert_true( CPFPHIG_OK == cpfphig_list_empty( &items, NULL ) );


    expect_any_count( cpfphig_free, Ptr, 5 );
    will_return_count( cpfphig_free, CPFPHIG_OK, 5 );
    assert_true( CPFPHIG_OK == cpfphig_binary_search_tree_empty( &tree, NULL ) );
}


int main( void )

{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(argument_tree_null),
        cmocka_unit_test(argument_items_null),

        cmocka_unit_test(copy_items_to_list),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
