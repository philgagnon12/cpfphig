#include "cpfphig/cpfphig.h"
#include "cpfphig/free.h"

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
    assert_int_equal( CPFPHIG_FAIL, cpfphig_free( NULL,
                                                NULL ) );

    printf("Ptr NULL with error\n");
    expect_value( cpfphig_error_message, Error_Type, cpfphig_system_error );
    expect_string( cpfphig_error_message, Message, "Ptr is NULL" );
    assert_int_equal( CPFPHIG_FAIL, cpfphig_free( NULL,
                                                &error ) );
}

static void deallocated( void** state )
{
    cpfphig* allocated_cpfphig = NULL;

    allocated_cpfphig = malloc( sizeof( cpfphig ) );
    assert_non_null( allocated_cpfphig );
    assert_int_equal( CPFPHIG_OK, cpfphig_free( &allocated_cpfphig,
                                              NULL ) );

    assert_null( allocated_cpfphig );
}

int main( void )
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(arguments),
        cmocka_unit_test(deallocated),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
