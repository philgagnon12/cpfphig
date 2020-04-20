#include "melphig/melphig.h"
#include "melphig/list.h"
#include "melphig_test/mock/list_mock.h"
#include "melphig/syntax_error_message.h"
#include "melphig/malloc.h"
#include "melphig/free.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>

static void arguments( void** state )
{
    struct mphig_list       list               = MELPHIG_CONST_MPHIG_LIST;
    int                     item               = 11;
    struct mphig_error      error              = {};

    printf("invalid List\n");
    assert_int_equal( MELPHIG_FAIL, real_mphig_list_shift( NULL,
                                                           &item,
                                                           NULL ) );

    printf("invalid List with error\n");
    expect_value( mphig_error_message, Error_Type, mphig_system_error );
    expect_string( mphig_error_message, Message, "List or Item is NULL");
    assert_int_equal( MELPHIG_FAIL, real_mphig_list_shift( NULL,
                                                           &item,
                                                           &error ) );

    printf("invalid Item\n");
    assert_int_equal( MELPHIG_FAIL, real_mphig_list_shift( &list,
                                                           NULL,
                                                           NULL ) );

    printf("invalid Item with error\n");
    expect_value( mphig_error_message, Error_Type, mphig_system_error );
    expect_string( mphig_error_message, Message, "List or Item is NULL");
    assert_int_equal( MELPHIG_FAIL, real_mphig_list_shift( &list,
                                                           NULL,
                                                           &error ) );

}

static void first_shift( void** state )
{
    struct mphig_list   list        = MELPHIG_CONST_MPHIG_LIST;
    int                 item        = 11;
    int*                shift_item  = NULL;


    expect_value( mphig_malloc, Size, sizeof( struct mphig_list_node ) );
    will_return( mphig_malloc, MELPHIG_OK );
    assert_int_equal(MELPHIG_OK, real_mphig_list_push( &list,
                                                       &item,
                                                       NULL ) );

    expect_any( mphig_free, Ptr );
    will_return( mphig_free, MELPHIG_OK );
    assert_int_equal(MELPHIG_OK, real_mphig_list_shift( &list,
                                                        &shift_item,
                                                        NULL ) );

    assert_null( list.first );
    assert_null( list.last );
    assert_int_equal( 11, *shift_item );

}

static void subsequent_shift( void** state )
{
    struct mphig_list       list                = MELPHIG_CONST_MPHIG_LIST;
    int                     first_item          = 11;
    int                     second_item         = 22;
    int                     third_item          = 33;
    int*                    first_shift_item    = NULL;
    int*                    second_shift_item   = NULL;
    int*                    third_shift_item    = NULL;

    expect_value( mphig_malloc, Size, sizeof( struct mphig_list_node ) );
    will_return( mphig_malloc, MELPHIG_OK );
    assert_int_equal(MELPHIG_OK, real_mphig_list_push( &list,
                                                       &first_item,
                                                       NULL ) );

    expect_value( mphig_malloc, Size, sizeof( struct mphig_list_node ) );
    will_return( mphig_malloc, MELPHIG_OK );
    assert_int_equal(MELPHIG_OK, real_mphig_list_push( &list,
                                                       &second_item,
                                                       NULL ) );

    expect_value( mphig_malloc, Size, sizeof( struct mphig_list_node ) );
    will_return( mphig_malloc, MELPHIG_OK );
    assert_int_equal(MELPHIG_OK, real_mphig_list_push( &list,
                                                       &third_item,
                                                       NULL ) );


    expect_any( mphig_free, Ptr );
    will_return( mphig_free, MELPHIG_OK );
    assert_int_equal(MELPHIG_OK, real_mphig_list_shift( &list,
                                                        &first_shift_item,
                                                        NULL ) );

    assert_non_null( list.first );
    assert_non_null( list.last );

    assert_non_null( list.first->next );
    assert_null(     list.first->previous );

    assert_null(    list.last->next );
    assert_non_null( list.last->previous );

    assert_int_equal( 11, *first_shift_item );
    assert_int_equal( 22, *(int*)(list.first->item) );
    assert_int_equal( 33, *(int*)(list.last->item) );

    expect_any( mphig_free, Ptr );
    will_return( mphig_free, MELPHIG_OK );
    assert_int_equal(MELPHIG_OK, real_mphig_list_shift( &list,
                                                        &second_shift_item,
                                                        NULL ) );

    assert_non_null( list.first );
    assert_non_null( list.last );

    assert_null( list.first->next );
    assert_null( list.first->previous );

    assert_null( list.last->next );
    assert_null( list.last->previous );

    assert_int_equal( 22, *second_shift_item );
    assert_int_equal( 33, *(int*)(list.first->item) );
    assert_int_equal( 33, *(int*)(list.last->item) );

    expect_any( mphig_free, Ptr );
    will_return( mphig_free, MELPHIG_OK );
    assert_int_equal(MELPHIG_OK, real_mphig_list_shift( &list,
                                                        &third_shift_item,
                                                        NULL ) );


    assert_null( list.first );
    assert_null( list.last );

    assert_int_equal( 33, *third_shift_item );
}

static void shift_null( void** state )
{
    struct mphig_list       list            = MELPHIG_CONST_MPHIG_LIST;
    int                     item            = 11;
    void*                   shift_item      = &item;

    expect_value( mphig_malloc, Size, sizeof( struct mphig_list_node ) );
    will_return( mphig_malloc, MELPHIG_OK );
    assert_int_equal(MELPHIG_OK, real_mphig_list_push( &list,
                                                       NULL,
                                                       NULL ) );


    expect_any( mphig_free, Ptr );
    will_return( mphig_free, MELPHIG_OK );
    assert_int_equal(MELPHIG_OK, real_mphig_list_shift( &list,
                                                        &shift_item,
                                                        NULL ) );

    assert_null( shift_item );
}

static void fail_free_is_fail( void** state )
{
    struct mphig_list       list            = MELPHIG_CONST_MPHIG_LIST;
    void*                   shift_item      = NULL;

    expect_value( mphig_malloc, Size, sizeof( struct mphig_list_node ) );
    will_return( mphig_malloc, MELPHIG_OK );
    assert_int_equal(MELPHIG_OK, real_mphig_list_push( &list,
                                                       NULL,
                                                       NULL ) );

    expect_any( mphig_free, Ptr );
    will_return( mphig_free, MELPHIG_FAIL );
    assert_int_equal(MELPHIG_FAIL, real_mphig_list_shift( &list,
                                                          &shift_item,
                                                          NULL ) );
}

static void shift_empty_list( void** state )
{
    struct mphig_list       list            = MELPHIG_CONST_MPHIG_LIST;
    void*                   shift_item      = NULL;
    struct mphig_error      error           = MELPHIG_CONST_MPHIG_ERROR;

    assert_int_equal(MELPHIG_FAIL, real_mphig_list_shift( &list,
                                                          &shift_item,
                                                          NULL ) );


    expect_value( mphig_error_message, Error_Type, mphig_user_error );
    expect_string( mphig_error_message, Message, "List is empty");
    assert_int_equal(MELPHIG_FAIL, real_mphig_list_shift( &list,
                                                          &shift_item,
                                                          &error ) );
}

int main( void )
{
    // TODO list_push_unit_test REQUIREMENT to PASS before theses tests get run
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(arguments),
        cmocka_unit_test(first_shift),
        cmocka_unit_test(subsequent_shift),
        cmocka_unit_test(shift_null),
        cmocka_unit_test(fail_free_is_fail),
        cmocka_unit_test(shift_empty_list),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
