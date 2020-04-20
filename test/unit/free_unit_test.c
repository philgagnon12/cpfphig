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
    struct fphig_error error = FPHIG_CONST_MPHIG_ERROR;

    printf("Ptr NULL\n");
    assert_int_equal( FPHIG_FAIL, fphig_free( NULL,
                                                NULL ) );

    printf("Ptr NULL with error\n");
    expect_value( fphig_error_message, Error_Type, fphig_system_error );
    expect_string( fphig_error_message, Message, "Ptr is NULL" );
    assert_int_equal( FPHIG_FAIL, fphig_free( NULL,
                                                &error ) );
}

static void deallocated( void** state )
{
    fphig* allocated_fphig = NULL;

    allocated_fphig = malloc( sizeof( fphig ) );
    assert_non_null( allocated_fphig );
    assert_int_equal( FPHIG_OK, fphig_free( &allocated_fphig,
                                              NULL ) );

    assert_null( allocated_fphig );
}

int main( void )
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(arguments),
        cmocka_unit_test(deallocated),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
