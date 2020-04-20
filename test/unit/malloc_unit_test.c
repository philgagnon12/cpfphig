#include "melphig/melphig.h"
#include "melphig/malloc.h"

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
    assert_int_equal( MELPHIG_FAIL, mphig_malloc( sizeof( mphig ),
                                                  NULL,
                                                  NULL ) );

    printf("Ptr NULL with error\n");
    expect_value( mphig_error_message, Error_Type, mphig_system_error );
    expect_string( mphig_error_message, Message, "Ptr is NULL" );
    assert_int_equal( MELPHIG_FAIL, mphig_malloc( sizeof( mphig ),
                                                  NULL,
                                                  &error ) );
}

static void allocated( void** state )
{
    mphig* allocated_mphig = NULL;

    assert_int_equal( MELPHIG_OK, mphig_malloc( sizeof( mphig ),
                                                &allocated_mphig,
                                                NULL ) );

    assert_non_null( allocated_mphig );
    *allocated_mphig = MELPHIG_OK;
    free( allocated_mphig );
}

int main( void )
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(arguments),
        cmocka_unit_test(allocated),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
