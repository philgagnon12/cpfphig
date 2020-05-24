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
    struct cpfphig_list     list               = CPFPHIG_CONST_CPFPHIG_LIST;
    int                     item               = 11;
    struct cpfphig_error    error              = CPFPHIG_CONST_CPFPHIG_ERROR;

    printf("invalid List\n");
    assert_int_equal( CPFPHIG_FAIL, real_cpfphig_list_shift( NULL,
                                                             &item,
                                                             NULL ) );

    printf("invalid List with error\n");
    expect_value( cpfphig_error_message_call, Error_Type, cpfphig_system_error );
    expect_string( cpfphig_error_message_call, Format, "List or Item is NULL");
    assert_int_equal( CPFPHIG_FAIL, real_cpfphig_list_shift( NULL,
                                                             &item,
                                                             &error ) );

    printf("invalid Item\n");
    assert_int_equal( CPFPHIG_FAIL, real_cpfphig_list_shift( &list,
                                                             NULL,
                                                             NULL ) );

    printf("invalid Item with error\n");
    expect_value( cpfphig_error_message_call, Error_Type, cpfphig_system_error );
    expect_string( cpfphig_error_message_call, Format, "List or Item is NULL");
    assert_int_equal( CPFPHIG_FAIL, real_cpfphig_list_shift( &list,
                                                             NULL,
                                                             &error ) );

}

static void first_shift( void** state )
{
    struct cpfphig_list   list      = CPFPHIG_CONST_CPFPHIG_LIST;
    int                 item        = 11;
    int*                shift_item  = NULL;


    expect_value( cpfphig_malloc, Size, sizeof( struct cpfphig_list_node ) );
    will_return( cpfphig_malloc, CPFPHIG_OK );
    assert_int_equal(CPFPHIG_OK, real_cpfphig_list_push( &list,
                                                         &item,
                                                         NULL ) );

    expect_any( cpfphig_free, Ptr );
    will_return( cpfphig_free, CPFPHIG_OK );
    assert_int_equal(CPFPHIG_OK, real_cpfphig_list_shift( &list,
                                                          &shift_item,
                                                          NULL ) );

    assert_null( list.first );
    assert_null( list.last );
    assert_int_equal( 11, *shift_item );

}

static void subsequent_shift( void** state )
{
    struct cpfphig_list       list              = CPFPHIG_CONST_CPFPHIG_LIST;
    int                     first_item          = 11;
    int                     second_item         = 22;
    int                     third_item          = 33;
    int*                    first_shift_item    = NULL;
    int*                    second_shift_item   = NULL;
    int*                    third_shift_item    = NULL;

    expect_value( cpfphig_malloc, Size, sizeof( struct cpfphig_list_node ) );
    will_return( cpfphig_malloc, CPFPHIG_OK );
    assert_int_equal(CPFPHIG_OK, real_cpfphig_list_push( &list,
                                                         &first_item,
                                                         NULL ) );

    expect_value( cpfphig_malloc, Size, sizeof( struct cpfphig_list_node ) );
    will_return( cpfphig_malloc, CPFPHIG_OK );
    assert_int_equal(CPFPHIG_OK, real_cpfphig_list_push( &list,
                                                         &second_item,
                                                         NULL ) );

    expect_value( cpfphig_malloc, Size, sizeof( struct cpfphig_list_node ) );
    will_return( cpfphig_malloc, CPFPHIG_OK );
    assert_int_equal(CPFPHIG_OK, real_cpfphig_list_push( &list,
                                                         &third_item,
                                                         NULL ) );


    expect_any( cpfphig_free, Ptr );
    will_return( cpfphig_free, CPFPHIG_OK );
    assert_int_equal(CPFPHIG_OK, real_cpfphig_list_shift( &list,
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

    expect_any( cpfphig_free, Ptr );
    will_return( cpfphig_free, CPFPHIG_OK );
    assert_int_equal(CPFPHIG_OK, real_cpfphig_list_shift( &list,
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

    expect_any( cpfphig_free, Ptr );
    will_return( cpfphig_free, CPFPHIG_OK );
    assert_int_equal(CPFPHIG_OK, real_cpfphig_list_shift( &list,
                                                        &third_shift_item,
                                                        NULL ) );


    assert_null( list.first );
    assert_null( list.last );

    assert_int_equal( 33, *third_shift_item );
}

static void shift_null( void** state )
{
    struct cpfphig_list     list            = CPFPHIG_CONST_CPFPHIG_LIST;
    int                     item            = 11;
    void*                   shift_item      = &item;

    expect_value( cpfphig_malloc, Size, sizeof( struct cpfphig_list_node ) );
    will_return( cpfphig_malloc, CPFPHIG_OK );
    assert_int_equal(CPFPHIG_OK, real_cpfphig_list_push( &list,
                                                         NULL,
                                                         NULL ) );


    expect_any( cpfphig_free, Ptr );
    will_return( cpfphig_free, CPFPHIG_OK );
    assert_int_equal(CPFPHIG_OK, real_cpfphig_list_shift( &list,
                                                          &shift_item,
                                                          NULL ) );

    assert_null( shift_item );
}

static void fail_free_is_fail( void** state )
{
    struct cpfphig_list list            = CPFPHIG_CONST_CPFPHIG_LIST;
    void*               shift_item      = NULL;

    expect_value( cpfphig_malloc, Size, sizeof( struct cpfphig_list_node ) );
    will_return( cpfphig_malloc, CPFPHIG_OK );
    assert_int_equal(CPFPHIG_OK, real_cpfphig_list_push( &list,
                                                         NULL,
                                                         NULL ) );

    expect_any( cpfphig_free, Ptr );
    will_return( cpfphig_free, CPFPHIG_FAIL );
    assert_int_equal(CPFPHIG_FAIL, real_cpfphig_list_shift( &list,
                                                            &shift_item,
                                                            NULL ) );
}

static void shift_empty_list( void** state )
{
    struct cpfphig_list     list        = CPFPHIG_CONST_CPFPHIG_LIST;
    void*                   shift_item  = NULL;
    struct cpfphig_error    error       = CPFPHIG_CONST_CPFPHIG_ERROR;

    assert_int_equal(CPFPHIG_FAIL, real_cpfphig_list_shift( &list,
                                                            &shift_item,
                                                            NULL ) );


    expect_value( cpfphig_error_message_call, Error_Type, cpfphig_user_error );
    expect_string( cpfphig_error_message_call, Format, "List is empty");
    assert_int_equal(CPFPHIG_FAIL, real_cpfphig_list_shift( &list,
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
