#include "melphig/melphig.h"
#include "melphig/list.h"
#include "melphig_test/mock/list_mock.h"
#include "melphig/malloc.h"
#include "melphig/free.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>

static void arguments( void** state )
{
    int                      item               = 11;
    struct mphig_error       error              = MELPHIG_CONST_MPHIG_ERROR;

    printf("invalid List\n");
    assert_int_equal( MELPHIG_FAIL, real_mphig_list_push( NULL,
                                                          &item,
                                                          NULL ) );

    printf("invalid List with error\n");
    expect_value( mphig_error_message, Error_Type, mphig_system_error );
    expect_string( mphig_error_message, Message, "List is NULL");
    assert_int_equal( MELPHIG_FAIL, real_mphig_list_push( NULL,
                                                          &item,
                                                          &error ) );
}

static void first_push( void** state )
{
    struct mphig_list   list    = MELPHIG_CONST_MPHIG_LIST;
    int                 item    = 11;

    expect_value(mphig_malloc, Size, sizeof( struct mphig_list_node ) );
    will_return( mphig_malloc, MELPHIG_OK );
    assert_int_equal(MELPHIG_OK, real_mphig_list_push( &list,
                                                       &item,
                                                       NULL ) );
    assert_non_null( list.first );
    assert_non_null( list.last );
    assert_null( list.first->next );
    assert_null( list.first->previous );

    expect_any( mphig_free, Ptr );
    will_return( mphig_free, MELPHIG_OK );
    assert_int_equal( MELPHIG_OK, mphig_free( &(list.first), NULL ) );
}

static void subsequent_push( void** state )
{
    struct mphig_list       list            = MELPHIG_CONST_MPHIG_LIST;
    int                     first_item      = 11;
    int                     second_item     = 22;

    expect_value(mphig_malloc, Size, sizeof( struct mphig_list_node ) );
    will_return( mphig_malloc, MELPHIG_OK );
    assert_int_equal(MELPHIG_OK, real_mphig_list_push( &list,
                                                       &first_item,
                                                       NULL ) );

    expect_value(mphig_malloc, Size, sizeof( struct mphig_list_node ) );
    will_return( mphig_malloc, MELPHIG_OK );
    assert_int_equal(MELPHIG_OK, real_mphig_list_push( &list,
                                                       &second_item,
                                                       NULL ) );

    assert_non_null( list.first );
    assert_non_null( list.last );

    assert_int_equal( 11, *(int*)(list.first->item) );

    assert_int_equal( 22, *(int*)(list.last->item) );

    assert_non_null( list.first->next );
    assert_null(     list.first->previous );

    assert_null(    list.last->next );
    assert_non_null( list.last->previous );

    assert_int_equal( 11, *(int*)(list.last->previous->item) );

    assert_int_equal( 22, *(int*)(list.first->next->item) );

    expect_any_count( mphig_free, Ptr, 2 );
    will_return_count( mphig_free, MELPHIG_OK, 2 );
    assert_int_equal( MELPHIG_OK, mphig_free( &(list.first->next), NULL ) );
    assert_int_equal( MELPHIG_OK, mphig_free( &(list.first), NULL ) );
}

static void push_null( void** state )
{
    struct mphig_list       list            = MELPHIG_CONST_MPHIG_LIST;

    expect_value(mphig_malloc, Size, sizeof( struct mphig_list_node ) );
    will_return( mphig_malloc, MELPHIG_OK );
    assert_int_equal(MELPHIG_OK, real_mphig_list_push( &list,
                                                       NULL,
                                                       NULL ) );

    assert_null( list.first->item );

    expect_any( mphig_free, Ptr );
    will_return( mphig_free, MELPHIG_OK );
    assert_int_equal( MELPHIG_OK, mphig_free( &(list.first), NULL ) );
}

static void fail_malloc_is_fail( void** state )
{
    struct mphig_list       list            = MELPHIG_CONST_MPHIG_LIST;
    int                     item            = 11;

    expect_value(mphig_malloc, Size, sizeof( struct mphig_list_node ) );
    will_return( mphig_malloc, MELPHIG_FAIL );
    assert_int_equal(MELPHIG_FAIL, real_mphig_list_push( &list,
                                                         &item,
                                                         NULL ) );
}

int main( void )
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(arguments),
        cmocka_unit_test(first_push),
        cmocka_unit_test(subsequent_push),
        cmocka_unit_test(push_null),
        cmocka_unit_test(fail_malloc_is_fail),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
