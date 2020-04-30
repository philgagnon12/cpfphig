#include "cpfphig/cpfphig.h"
#include "cpfphig/dirname.h"
#include "cpfphig/library_load.h"
#include "cpfphig/library_sym.h"
#include "cpfphig/library_unload.h"

#include <string.h>
#include <stdio.h>

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>


static void load_sym_unload( void** state )
{
    void* handle        = NULL;
    int(*symbol)(int)   = NULL;


    assert_true( CPFPHIG_OK == cpfphig_library_load( CPFPHIG_LIBRARY_PATH,
                                                     &handle,
                                                     NULL ) );

    assert_non_null( handle );

    assert_true( CPFPHIG_OK == cpfphig_library_sym( handle,
                                                    "library_symbol",
                                                    (void**)&symbol,
                                                    NULL ) );

    assert_non_null( symbol );

    assert_int_equal( 111, symbol(111) );

    assert_true( CPFPHIG_OK == cpfphig_library_unload( handle,
                                                       NULL ) );
}

int main( int argc, char* argv[]  )
{

    const struct CMUnitTest tests[] = {
        cmocka_unit_test(load_sym_unload),

    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
