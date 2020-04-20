#include "melphig/melphig.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <string.h>

static void arguments( void** state )
{
    struct fphig_error error = {};

    printf("invalid Message\n");
    assert_int_equal( FPHIG_FAIL,
                      fphig_error_message( fphig_system_error,
                                           NULL,
                                           &error,
                                           NULL,
                                           NULL,
                                           0 ) );

    printf("invalid Error\n");
    assert_int_equal( FPHIG_FAIL,
                      fphig_error_message( fphig_system_error,
                                           "error message",
                                           NULL,
                                           NULL,
                                           NULL,
                                           0) );
}

static void error_type_into_error( void** state )
{
    struct fphig_error error = {};

    printf("error_type same as arg\n");
    assert_int_equal( FPHIG_OK,
                      fphig_error_message( fphig_system_error,
                                           "error_message",
                                           &error,
                                           NULL,
                                           NULL,
                                           0 ) );

    assert_int_equal( fphig_system_error, error.error_type );
}

static void message_into_error( void** state )
{
    struct fphig_error error = {};

    printf("message with valid message_size and exact string length\n");
    assert_int_equal( FPHIG_OK,
                      fphig_error_message( fphig_system_error,
                                           "error_message",
                                           &error,
                                           NULL,
                                           NULL,
                                           0 ) );

    assert_string_equal( "error_message", error.message );
    assert_true( error.message_size > 13 );
    assert_true( strnlen( error.message, error.message_size ) > 12 );
    assert_true( strnlen( error.message, error.message_size ) < 14 );
}

static void file_into_error( void** state )
{
    struct fphig_error error = {};

    printf("file with valid file_size and exact string length\n");
    assert_int_equal( FPHIG_OK,
                      fphig_error_message( fphig_system_error,
                                           "error_message",
                                           &error,
                                           "file.c",
                                           NULL,
                                           0 ) );

    assert_string_equal("file.c", error.file );
    assert_true( error.file_size > 6 );
    assert_true( strnlen( error.file, error.file_size ) > 5 );
    assert_true( strnlen( error.file, error.file_size ) < 7 );

}

static void function_into_error( void** state )
{
    struct fphig_error error = {};

    printf("function with valid function_size and exact string length\n");
    assert_int_equal( FPHIG_OK,
                      fphig_error_message( fphig_system_error,
                                           "error_message",
                                           &error,
                                           NULL,
                                           "function",
                                           0 ) );

    assert_string_equal("function", error.function );
    assert_true( error.function_size > 8 );
    assert_true( strnlen( error.function, error.function_size ) > 7 );
    assert_true( strnlen( error.function, error.function_size ) < 9 );
}

static void line_into_error( void** state )
{
    struct fphig_error error = {};

    printf("line same as arg\n");
    assert_int_equal( FPHIG_OK,
                      fphig_error_message( fphig_system_error,
                                           "error_message",
                                           &error,
                                           NULL,
                                           NULL,
                                           44 ) );

    assert_int_equal( 44, error.line );
}

int main( void )
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(arguments),
        cmocka_unit_test(error_type_into_error),
        cmocka_unit_test(message_into_error),
        cmocka_unit_test(file_into_error),
        cmocka_unit_test(function_into_error),
        cmocka_unit_test(line_into_error),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
