#include "fphig/fphig.h"
#include "fphig/malloc.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <stdlib.h>

static void arguments( void** state )
{
    struct fphig_error error = FPHIG_CONST_FPHIG_ERROR;

    printf("Ptr NULL\n");
    assert_int_equal( FPHIG_FAIL, fphig_malloc( sizeof( fphig ),
                                                  NULL,
                                                  NULL ) );

    printf("Ptr NULL with error\n");
    expect_value( fphig_error_message, Error_Type, fphig_system_error );
    expect_string( fphig_error_message, Message, "Ptr is NULL" );
    assert_int_equal( FPHIG_FAIL, fphig_malloc( sizeof( fphig ),
                                                  NULL,
                                                  &error ) );
}

static void allocated( void** state )
{
    fphig* allocated_fphig = NULL;

    assert_int_equal( FPHIG_OK, fphig_malloc( sizeof( fphig ),
                                                &allocated_fphig,
                                                NULL ) );

    assert_non_null( allocated_fphig );
    *allocated_fphig = FPHIG_OK;
    free( allocated_fphig );
}

int main( void )
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(arguments),
        cmocka_unit_test(allocated),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
