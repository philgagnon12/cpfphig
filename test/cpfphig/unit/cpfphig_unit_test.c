#include "cpfphig/cpfphig.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <string.h>


#define CPFPHIG_BUFFER_SIZE ( 0x0F )

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
    char                 file_buffer[ CPFPHIG_BUFFER_SIZE ];

    // Reset
    memset( line_buffer,
            0x00,
            CPFPHIG_BUFFER_SIZE );

    memset( file_buffer,
            0x00,
            CPFPHIG_BUFFER_SIZE );


    assert_true( CPFPHIG_OK == cpfphig_error_message( cpfphig_system_error,
                                                      "%s",
                                                      &error,
                                                      "Test") );

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

    // Expect first charcter of this file to  be at stderr_pos
    fseek( stderr, error.stderr_pos, SEEK_SET );
    fread( file_buffer, sizeof( char ), sizeof( char ), stderr );
    assert_true( file_buffer[0] == __FILE__[0] );
    // Reposition stderr
    fseek( stderr, 0, SEEK_END );
}
int main( void )
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(arguments),
        cmocka_unit_test(error_type_and_pos_and_len_into_error),

    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
