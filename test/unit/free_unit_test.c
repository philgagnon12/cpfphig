#include "melphig/melphig.h"
#include "melphig/free.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <stdlib.h>

static void arguments( void** state )
{
    struct mphig_error error = MELPHIG_CONST_MPHIG_ERROR;

    printf("Ptr NULL\n");
    assert_int_equal( MELPHIG_FAIL, mphig_free( NULL,
                                                NULL ) );

    printf("Ptr NULL with error\n");
    expect_value( mphig_error_message, Error_Type, mphig_system_error );
    expect_string( mphig_error_message, Message, "Ptr is NULL" );
    assert_int_equal( MELPHIG_FAIL, mphig_free( NULL,
                                                &error ) );
}

static void deallocated( void** state )
{
    mphig* allocated_mphig = NULL;

    allocated_mphig = malloc( sizeof( mphig ) );
    assert_non_null( allocated_mphig );
    assert_int_equal( MELPHIG_OK, mphig_free( &allocated_mphig,
                                              NULL ) );

    assert_null( allocated_mphig );
}

int main( void )
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(arguments),
        cmocka_unit_test(deallocated),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
