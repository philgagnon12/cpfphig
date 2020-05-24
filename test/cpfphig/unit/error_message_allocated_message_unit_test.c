#include "cpfphig/cpfphig.h"
#include "cpfphig/error_message_allocated_message.h"

#include "cpfphig/free.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <string.h>

#include <stdlib.h>
#include <stdarg.h>

static
cpfphig
va_error_message_allocated_message( struct cpfphig_error*            Error,
                                    const char*                      Format,
                                    ... )
{
    cpfphig ret = CPFPHIG_FAIL;

    va_list args;
    va_start( args, Format );
    ret = cpfphig_error_message_allocated_message( Error, Format, args );
    va_end( args );
    return ret;
}

static void arguments( void** state )
{
    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ALLOCATED_MESSAGE_ERROR;

    assert_true( CPFPHIG_FAIL == va_error_message_allocated_message( NULL,
                                                                     "error_message" ) );

    assert_true( CPFPHIG_FAIL == va_error_message_allocated_message( &error,
                                                                     NULL ) );
}
static void allocated_message( void** state )
{
    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ALLOCATED_MESSAGE_ERROR;

    expect_value( cpfphig_malloc, Size, sizeof("error_message") + sizeof("test") - sizeof( char ) );
    will_return( cpfphig_malloc, CPFPHIG_OK );

    assert_true( CPFPHIG_OK == va_error_message_allocated_message( &error,
                                                                   "error_message%s",
                                                                   "test") );

    assert_string_equal( "error_messagetest", error.error_component.allocated_message.message );

    expect_any( cpfphig_free, Ptr );
    will_return( cpfphig_free, CPFPHIG_OK );
    assert_true( CPFPHIG_OK == cpfphig_free( &error.error_component.allocated_message.message, NULL ) );
}


int main( void )
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(arguments),
        cmocka_unit_test(allocated_message),

    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
