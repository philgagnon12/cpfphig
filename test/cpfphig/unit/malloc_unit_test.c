#include "cpfphig/cpfphig.h"
#include "cpfphig/malloc.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <stdlib.h>

static void arguments( void** state )
{
    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;

    printf("Ptr NULL\n");
    assert_int_equal( CPFPHIG_FAIL, cpfphig_malloc( sizeof( cpfphig ),
                                                    NULL,
                                                    NULL ) );

    printf("Ptr NULL with error\n");
    expect_value( cpfphig_error_message_call, Error_Type, cpfphig_system_error );
    expect_string( cpfphig_error_message_call, Format, "Ptr is NULL" );
    assert_int_equal( CPFPHIG_FAIL, cpfphig_malloc( sizeof( cpfphig ),
                                                    NULL,
                                                    &error ) );
}

static void allocated( void** state )
{
    cpfphig* allocated_cpfphig = NULL;

    assert_int_equal( CPFPHIG_OK, cpfphig_malloc( sizeof( cpfphig ),
                                                  &allocated_cpfphig,
                                                  NULL ) );

    assert_non_null( allocated_cpfphig );
    *allocated_cpfphig = CPFPHIG_OK;
    free( allocated_cpfphig );
}

int main( void )
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(arguments),
        cmocka_unit_test(allocated),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
