#include "cpfphig/cpfphig.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <string.h>

static void arguments( void** state )
{
    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;

    assert_true( CPFPHIG_FAIL == cpfphig_error_message( cpfphig_system_error,
                                                        NULL,
                                                        &error ) );

    assert_true( CPFPHIG_FAIL == cpfphig_error_message( cpfphig_system_error,
                                                        "error message",
                                                        NULL ) );
}

static void call_error_message_fprintf( void** state )
{
    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;

    expect_string( cpfphig_error_message_fprintf, Format, "error message" );
    assert_true( CPFPHIG_OK == cpfphig_error_message( cpfphig_system_error,
                                                      "error message",
                                                      &error ) );
}

static void call_error_message_allocated_message( void** state )
{
    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;
    error.error_component_type = cpfphig_error_allocated_message;

    expect_string( cpfphig_error_message_allocated_message, Format, "error message" );
    assert_true( CPFPHIG_OK == cpfphig_error_message( cpfphig_system_error,
                                                      "error message",
                                                      &error ) );
}

int main( void )
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(arguments),
        cmocka_unit_test(call_error_message_fprintf),
        cmocka_unit_test(call_error_message_allocated_message),

    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
