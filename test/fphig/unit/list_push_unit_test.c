#include "fphig/fphig.h"
#include "fphig/list.h"
#include "fphig/mock/list_mock.h"
#include "fphig/malloc.h"
#include "fphig/free.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>

static void arguments( void** state )
{
    int                      item               = 11;
    struct fphig_error       error              = FPHIG_CONST_FPHIG_ERROR;

    printf("invalid List\n");
    assert_int_equal( FPHIG_FAIL, real_fphig_list_push( NULL,
                                                          &item,
                                                          NULL ) );

    printf("invalid List with error\n");
    expect_value( fphig_error_message, Error_Type, fphig_system_error );
    expect_string( fphig_error_message, Message, "List is NULL");
    assert_int_equal( FPHIG_FAIL, real_fphig_list_push( NULL,
                                                          &item,
                                                          &error ) );
}

static void first_push( void** state )
{
    struct fphig_list   list    = FPHIG_CONST_FPHIG_LIST;
    int                 item    = 11;

    expect_value(fphig_malloc, Size, sizeof( struct fphig_list_node ) );
    will_return( fphig_malloc, FPHIG_OK );
    assert_int_equal(FPHIG_OK, real_fphig_list_push( &list,
                                                       &item,
                                                       NULL ) );
    assert_non_null( list.first );
    assert_non_null( list.last );
    assert_null( list.first->next );
    assert_null( list.first->previous );

    expect_any( fphig_free, Ptr );
    will_return( fphig_free, FPHIG_OK );
    assert_int_equal( FPHIG_OK, fphig_free( &(list.first), NULL ) );
}

static void subsequent_push( void** state )
{
    struct fphig_list       list            = FPHIG_CONST_FPHIG_LIST;
    int                     first_item      = 11;
    int                     second_item     = 22;

    expect_value(fphig_malloc, Size, sizeof( struct fphig_list_node ) );
    will_return( fphig_malloc, FPHIG_OK );
    assert_int_equal(FPHIG_OK, real_fphig_list_push( &list,
                                                       &first_item,
                                                       NULL ) );

    expect_value(fphig_malloc, Size, sizeof( struct fphig_list_node ) );
    will_return( fphig_malloc, FPHIG_OK );
    assert_int_equal(FPHIG_OK, real_fphig_list_push( &list,
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

    expect_any_count( fphig_free, Ptr, 2 );
    will_return_count( fphig_free, FPHIG_OK, 2 );
    assert_int_equal( FPHIG_OK, fphig_free( &(list.first->next), NULL ) );
    assert_int_equal( FPHIG_OK, fphig_free( &(list.first), NULL ) );
}

static void push_null( void** state )
{
    struct fphig_list       list            = FPHIG_CONST_FPHIG_LIST;

    expect_value(fphig_malloc, Size, sizeof( struct fphig_list_node ) );
    will_return( fphig_malloc, FPHIG_OK );
    assert_int_equal(FPHIG_OK, real_fphig_list_push( &list,
                                                       NULL,
                                                       NULL ) );

    assert_null( list.first->item );

    expect_any( fphig_free, Ptr );
    will_return( fphig_free, FPHIG_OK );
    assert_int_equal( FPHIG_OK, fphig_free( &(list.first), NULL ) );
}

static void fail_malloc_is_fail( void** state )
{
    struct fphig_list       list            = FPHIG_CONST_FPHIG_LIST;
    int                     item            = 11;

    expect_value(fphig_malloc, Size, sizeof( struct fphig_list_node ) );
    will_return( fphig_malloc, FPHIG_FAIL );
    assert_int_equal(FPHIG_FAIL, real_fphig_list_push( &list,
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
