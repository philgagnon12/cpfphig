#include "melphig/melphig.h"
#include "melphig/list.h"
#include "melphig_test/mock/list_mock.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>

static void arguments( void** state )
{
    struct mphig_list_iterator  list_iterator = MELPHIG_CONST_MPHIG_LIST_ITERATOR;
    int*                        item                = NULL;
    struct mphig_error          error               = MELPHIG_CONST_MPHIG_ERROR;

    printf("invalid List_Iterator\n");
    assert_int_equal( MELPHIG_FAIL, real_mphig_list_next( NULL,
                                                          &item,
                                                          NULL ) );

    printf("invalid List_Iterator with error\n");
    expect_value( mphig_error_message, Error_Type, mphig_system_error );
    expect_string( mphig_error_message, Message, "List_Iterator or Item is NULL");
    assert_int_equal( MELPHIG_FAIL, real_mphig_list_next( NULL,
                                                          &item,
                                                          &error ) );

    printf("invalid Item\n");
    assert_int_equal( MELPHIG_FAIL, real_mphig_list_next( &list_iterator,
                                                          NULL,
                                                          NULL ) );

    printf("invalid Item with error\n");
    expect_value( mphig_error_message, Error_Type, mphig_system_error );
    expect_string( mphig_error_message, Message, "List_Iterator or Item is NULL");
    assert_int_equal( MELPHIG_FAIL, real_mphig_list_next( &list_iterator,
                                                          NULL,
                                                          &error ) );

}

static void list_null( void** state )
{
    struct mphig_list_iterator  list_iterator   = MELPHIG_CONST_MPHIG_LIST_ITERATOR;
    int*                        item            = NULL;
    struct mphig_error          error           = MELPHIG_CONST_MPHIG_ERROR;

    printf("list_null\n");
    assert_int_equal( MELPHIG_FAIL, real_mphig_list_next( &list_iterator,
                                                          &item,
                                                          NULL ) );

    printf("list_null with error\n");
    expect_value( mphig_error_message, Error_Type, mphig_system_error );
    expect_string( mphig_error_message, Message, "List_Iterator->list is NULL");
    assert_int_equal( MELPHIG_FAIL, real_mphig_list_next( &list_iterator,
                                                          &item,
                                                          &error ) );
}

static void empty_list( void** state )
{
    struct mphig_list           list          = MELPHIG_CONST_MPHIG_LIST;
    struct mphig_list_iterator  list_iterator = { &list, NULL };
    int*                        item          = NULL;
    struct mphig_error          error         = MELPHIG_CONST_MPHIG_ERROR;

    printf("empty_list\n");
    assert_int_equal( MELPHIG_FAIL, real_mphig_list_next( &list_iterator,
                                                          &item,
                                                          NULL ) );

    printf("empty_list with error\n");
    expect_value( mphig_error_message, Error_Type, mphig_user_error );
    expect_string( mphig_error_message, Message, "End of list has been reached");
    assert_int_equal( MELPHIG_FAIL, real_mphig_list_next( &list_iterator,
                                                          &item,
                                                          &error ) );
}

static void next_item_is_null( void** state )
{
    struct mphig_list_iterator  list_iterator   = MELPHIG_CONST_MPHIG_LIST_ITERATOR;
    struct mphig_list           list            = MELPHIG_CONST_MPHIG_LIST;
    struct mphig_list_node      node            = MELPHIG_CONST_MPHIG_LIST_NODE;
    int*                        item            = NULL;
    struct mphig_error          error           = MELPHIG_CONST_MPHIG_ERROR;

    list.first = &node;
    list_iterator.list = &list;

    printf("next_item_is_null\n");
    assert_int_equal( MELPHIG_FAIL, real_mphig_list_next( &list_iterator,
                                                          &item,
                                                          NULL ) );


    // Reset iterator positionto start from first
    list_iterator.current_node = NULL;

    printf("next_item_is_null with error\n");
    expect_value( mphig_error_message, Error_Type, mphig_system_error );
    expect_string( mphig_error_message, Message, "current_node->item is NULL");
    assert_int_equal( MELPHIG_FAIL, real_mphig_list_next( &list_iterator,
                                                          &item,
                                                          &error ) );
}

static void next_item( void** state )
{
    struct mphig_list_iterator  list_iterator = MELPHIG_CONST_MPHIG_LIST_ITERATOR;
    struct mphig_list           list          = MELPHIG_CONST_MPHIG_LIST;
    struct mphig_list_node      node          = MELPHIG_CONST_MPHIG_LIST_NODE;
    int*                        item          = NULL;
    int                         number        = 11;
    struct mphig_error          error         = MELPHIG_CONST_MPHIG_ERROR;

    node.item  = &number;
    list.first = &node;
    list_iterator.list = &list;

    assert_int_equal( MELPHIG_OK, real_mphig_list_next( &list_iterator,
                                                        &item,
                                                        &error ) );

    assert_non_null( item );
    assert_int_equal( 11, *item );

    expect_value( mphig_error_message, Error_Type, mphig_user_error );
    expect_string( mphig_error_message, Message, "End of list has been reached");

    assert_int_equal( MELPHIG_FAIL, real_mphig_list_next( &list_iterator,
                                                          &item,
                                                          &error ) );

    // item is not being reset after reaching the end of list
    assert_non_null( item );
    assert_null( list_iterator.current_node );
}

static void list_iterator_rewinds( void** state )
{
    struct mphig_list_iterator  list_iterator = MELPHIG_CONST_MPHIG_LIST_ITERATOR;
    struct mphig_list           list          = MELPHIG_CONST_MPHIG_LIST;
    struct mphig_list_node      node          = MELPHIG_CONST_MPHIG_LIST_NODE;
    int*                        item          = NULL;
    int                         number        = 11;
    struct mphig_error          error         = MELPHIG_CONST_MPHIG_ERROR;

    node.item  = &number;
    list.first = &node;
    list_iterator.list = &list;

    assert_int_equal( MELPHIG_OK, real_mphig_list_next( &list_iterator,
                                                        &item,
                                                        &error ) );

    assert_non_null( item );
    assert_int_equal( 11, *item );

    expect_value( mphig_error_message, Error_Type, mphig_user_error );
    expect_string( mphig_error_message, Message, "End of list has been reached");

    assert_int_equal( MELPHIG_FAIL, real_mphig_list_next( &list_iterator,
                                                          &item,
                                                          &error ) );

    // item is not being reset after reaching the end of list
    assert_non_null( item );
    assert_null( list_iterator.current_node );

    assert_int_equal( MELPHIG_OK, real_mphig_list_next( &list_iterator,
                                                        &item,
                                                        &error ) );

    assert_non_null( item );
    assert_int_equal( 11, *item );

}

int main( void )
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(arguments),
        cmocka_unit_test(list_null),
        cmocka_unit_test(empty_list),
        cmocka_unit_test(next_item_is_null),
        cmocka_unit_test(next_item),
        cmocka_unit_test(list_iterator_rewinds),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
