#include "melphig/melphig.h"
#include "melphig/directory_list.h"
#include "melphig/destroy_directory_list.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <string.h>

#ifdef MELPHIG_HAVE_LIBGEN_H
#include <libgen.h>
#endif

static char* bin_name   = NULL;
static char* bin_dir    = NULL;

static void list_at_least_this_binary( void** state )
{
    struct mphig_list           file_names          = MELPHIG_CONST_MPHIG_LIST;
    struct mphig_list_iterator  list_iterator       = { &file_names, NULL };
    char*                       file_name           = NULL;
    int                         file_names_count    = 0;
    int                         this_bin_count      = 0;

    assert_int_equal( MELPHIG_OK, mphig_directory_list( bin_dir,
                                                        &file_names,
                                                        NULL ) );

    while( MELPHIG_OK == mphig_list_next( &list_iterator,
                                          &file_name,
                                          NULL ) )
    {
        if( 0 == strncmp( bin_name, file_name, 0xFF ) )
        {
            this_bin_count++;
        }

        file_names_count++;
    }

    assert_int_not_equal( 0, file_names_count );
    assert_int_equal( 1, this_bin_count );
    assert_int_equal( MELPHIG_OK, mphig_destroy_directory_list( &file_names, NULL ) );
}

int main( int argc, char* argv[]  )
{
    #ifdef MELPHIG_HAVE_LIBGEN_H
    bin_name    = basename(argv[0]);
    bin_dir     = dirname(argv[0]);
    #endif

    // windows will be _splitpath

    const struct CMUnitTest tests[] = {
        cmocka_unit_test(list_at_least_this_binary),

    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
