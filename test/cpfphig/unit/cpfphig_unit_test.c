#include "cpfphig/cpfphig.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <string.h>

#include <stdlib.h>

#ifdef CPFPHIG_HAVE_UNISTD_H
#include <unistd.h>
#else
#include <windows.h>
#endif

#define CPFPHIG_BUFFER_SIZE ( 0x0F )
#define CPFPHIG_STDERR_BUFFER_SIZE ( 0xFF )

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

static void error_type_and_pos_and_len_into_error( void** state )
{
    struct cpfphig_error error          = CPFPHIG_CONST_CPFPHIG_ERROR;
    int                  expected_len   = 0;
    char                 line_buffer[ CPFPHIG_BUFFER_SIZE ];
    char*                stderr_buffer = NULL;

    stderr_buffer = malloc( CPFPHIG_STDERR_BUFFER_SIZE );
    assert_non_null( stderr_buffer );

    // Reset
    memset( line_buffer,
            0x00,
            CPFPHIG_BUFFER_SIZE );

    memset( stderr_buffer,
            0x00,
            CPFPHIG_STDERR_BUFFER_SIZE );

    // print into buffer
    assert_true( 0 == setvbuf( stderr, stderr_buffer, _IOFBF, CPFPHIG_STDERR_BUFFER_SIZE ) );

    // Hide stderr
    close( STDERR_FILENO );

    assert_true( CPFPHIG_OK == cpfphig_error_message( cpfphig_system_error,
                                                      "%s",
                                                      &error,
                                                      "Test") );

    fflush( stderr );

    assert_int_equal( cpfphig_system_error, error.error_type );

    expected_len =  sizeof( __FILE__ ) - sizeof( char ); // __FILE__ without null char
    expected_len += sizeof( char ); // '('
    expected_len +=  sizeof( __FUNCTION__ ) - sizeof( char ); // __FUNCTION__ without null char
    expected_len += sizeof( char ) * 2; // '):'

    // We expect that the __LINE__ that we snprintf right here will be same length as the one from the above call to cpfphig_error_message
    expected_len += snprintf( line_buffer,
                              CPFPHIG_BUFFER_SIZE,
                              "%d",
                              __LINE__ );

    expected_len += sizeof( char ) * 2; // '; '
    expected_len +=  sizeof( "Test" ) - sizeof( char ); // "Test" without null char
    expected_len += sizeof( char ); // '\n'

    assert_int_equal( expected_len, error.log_len );

    assert_memory_equal( "Test", (stderr_buffer + expected_len) - sizeof( "Test" ), sizeof( "Test" ) - sizeof( char ) );
    assert_true( 0 == setvbuf( stderr, NULL, _IONBF, 0 ) );
    free( stderr_buffer );
}

int main( void )
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(arguments),
        cmocka_unit_test(error_type_and_pos_and_len_into_error),

    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
