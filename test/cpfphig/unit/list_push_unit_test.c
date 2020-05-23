#include "cpfphig/cpfphig.h"
#include "cpfphig/list.h"
#include "cpfphig/unit/mock/list_mock.h"
#include "cpfphig/malloc.h"
#include "cpfphig/free.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>

static void arguments( void** state )
{
    int                     item    = 11;
    struct cpfphig_error    error   = CPFPHIG_CONST_CPFPHIG_ERROR;

    printf("invalid List\n");
    assert_int_equal( CPFPHIG_FAIL, real_cpfphig_list_push( NULL,
                                                            &item,
                                                            NULL ) );

    printf("invalid List with error\n");
    expect_value( cpfphig_stderr_printf, Error_Type, cpfphig_system_error );
    expect_string( cpfphig_stderr_printf, Format, "List is NULL");
    assert_int_equal( CPFPHIG_FAIL, real_cpfphig_list_push( NULL,
                                                            &item,
                                                            &error ) );
}

static void first_push( void** state )
{
    struct cpfphig_list   list    = CPFPHIG_CONST_CPFPHIG_LIST;
    int                 item    = 11;

    expect_value(cpfphig_malloc, Size, sizeof( struct cpfphig_list_node ) );
    will_return( cpfphig_malloc, CPFPHIG_OK );
    assert_int_equal(CPFPHIG_OK, real_cpfphig_list_push( &list,
                                                         &item,
                                                         NULL ) );
    assert_non_null( list.first );
    assert_non_null( list.last );
    assert_null( list.first->next );
    assert_null( list.first->previous );

    expect_any( cpfphig_free, Ptr );
    will_return( cpfphig_free, CPFPHIG_OK );
    assert_int_equal( CPFPHIG_OK, cpfphig_free( &(list.first), NULL ) );
}

static void subsequent_push( void** state )
{
    struct cpfphig_list list            = CPFPHIG_CONST_CPFPHIG_LIST;
    int                 first_item      = 11;
    int                 second_item     = 22;

    expect_value(cpfphig_malloc, Size, sizeof( struct cpfphig_list_node ) );
    will_return( cpfphig_malloc, CPFPHIG_OK );
    assert_int_equal(CPFPHIG_OK, real_cpfphig_list_push( &list,
                                                         &first_item,
                                                         NULL ) );

    expect_value(cpfphig_malloc, Size, sizeof( struct cpfphig_list_node ) );
    will_return( cpfphig_malloc, CPFPHIG_OK );
    assert_int_equal(CPFPHIG_OK, real_cpfphig_list_push( &list,
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

    expect_any_count( cpfphig_free, Ptr, 2 );
    will_return_count( cpfphig_free, CPFPHIG_OK, 2 );
    assert_int_equal( CPFPHIG_OK, cpfphig_free( &(list.first->next), NULL ) );
    assert_int_equal( CPFPHIG_OK, cpfphig_free( &(list.first), NULL ) );
}

static void push_null( void** state )
{
    struct cpfphig_list list    = CPFPHIG_CONST_CPFPHIG_LIST;

    expect_value(cpfphig_malloc, Size, sizeof( struct cpfphig_list_node ) );
    will_return( cpfphig_malloc, CPFPHIG_OK );
    assert_int_equal(CPFPHIG_OK, real_cpfphig_list_push( &list,
                                                         NULL,
                                                         NULL ) );

    assert_null( list.first->item );

    expect_any( cpfphig_free, Ptr );
    will_return( cpfphig_free, CPFPHIG_OK );
    assert_int_equal( CPFPHIG_OK, cpfphig_free( &(list.first), NULL ) );
}

static void fail_malloc_is_fail( void** state )
{
    struct cpfphig_list list            = CPFPHIG_CONST_CPFPHIG_LIST;
    int                 item            = 11;

    expect_value(cpfphig_malloc, Size, sizeof( struct cpfphig_list_node ) );
    will_return( cpfphig_malloc, CPFPHIG_FAIL );
    assert_int_equal(CPFPHIG_FAIL, real_cpfphig_list_push( &list,
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
