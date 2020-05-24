#include "cpfphig/cpfphig.h"
#include "cpfphig/error_message_fprintf.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <string.h>

#include <stdlib.h>
#include <stdarg.h>

#ifdef CPFPHIG_HAVE_UNISTD_H
#include <unistd.h>
#else
#include <windows.h>
#include <io.h>
#define close _close
#define fileno _fileno
#endif

#define CPFPHIG_BUFFER_SIZE ( 0x0F )
#define CPFPHIG_STDERR_BUFFER_SIZE ( 0xFF )

static
cpfphig
va_error_message_fprintf( struct cpfphig_error*            Error,
                          const char*                      Format,
                          ... )
{
    cpfphig ret = CPFPHIG_FAIL;

    va_list args;
    va_start( args, Format );
    ret = cpfphig_error_message_fprintf( Error, Format, args );
    va_end( args );
    return ret;
}

static void arguments( void** state )
{
    struct cpfphig_error error = CPFPHIG_CONST_CPFPHIG_ERROR;

    assert_true( CPFPHIG_FAIL == va_error_message_fprintf( NULL,
                                                           "error_message" ) );

    assert_true( CPFPHIG_FAIL == va_error_message_fprintf( &error,
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
    // TODO need to do actual redirection instead of close
    // close( fileno( stderr ) );

    error.file     = __FILE__;
    error.function = __FUNCTION__;
    error.line     = __LINE__;

    assert_true( CPFPHIG_OK == va_error_message_fprintf( &error,
                                                         "%s",
                                                         "Test" ) );

    fflush( stderr );

    expected_len =  sizeof( __FILE__ ) - sizeof( char ); // __FILE__ without null char
    expected_len += sizeof( char ); // '('
    expected_len +=  sizeof( __FUNCTION__ ) - sizeof( char ); // __FUNCTION__ without null char
    expected_len += sizeof( char ) * 2; // '):'

    // We expect that the __LINE__ that we snprintf right here will be same length as the one from the above call to va_error_message_fprintf
    expected_len += snprintf( line_buffer,
                              CPFPHIG_BUFFER_SIZE,
                              "%d",
                              __LINE__ );

    expected_len += sizeof( char ) * 2; // '; '
    expected_len +=  sizeof( "Test" ) - sizeof( char ); // "Test" without null char
    expected_len += sizeof( char ); // '\n'

    assert_int_equal( expected_len, error.error_component.fprintf.log_len );

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
