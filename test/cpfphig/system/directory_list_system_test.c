#include "cpfphig/cpfphig.h"
#include "cpfphig/directory_list.h"
#include "cpfphig/destroy_directory_list.h"
#include "cpfphig/basename.h"
#include "cpfphig/dirname.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <string.h>

#define CPFPHIG_BUFFER_SIZE ( 0x0AFF )

static char bin_name[CPFPHIG_BUFFER_SIZE];
static char bin_dir[CPFPHIG_BUFFER_SIZE];


static void list_at_least_this_binary( void** state )
{
    struct cpfphig_list             file_names          = CPFPHIG_CONST_CPFPHIG_LIST;
    struct cpfphig_list_iterator    list_iterator       = { &file_names, NULL };
    char*                           file_name           = NULL;
    int                             file_names_count    = 0;
    int                             this_bin_count      = 0;

    assert_int_equal( CPFPHIG_OK, cpfphig_directory_list( bin_dir,
                                                          &file_names,
                                                          NULL ) );

    while( CPFPHIG_OK == cpfphig_list_next( &list_iterator,
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
    assert_int_equal( CPFPHIG_OK, cpfphig_destroy_directory_list( &file_names, NULL ) );
}

int main( int argc, char* argv[]  )
{
    memset( bin_name,
            0x00,
            CPFPHIG_BUFFER_SIZE );

    memset( bin_dir,
            0x00,
            CPFPHIG_BUFFER_SIZE );

    if( CPFPHIG_FAIL == cpfphig_basename( argv[0],
                                          strlen( argv[0] ) + 1,
                                          bin_name,
                                          CPFPHIG_BUFFER_SIZE,
                                          NULL ) )
    {
        return CPFPHIG_FAIL;
    }

    if( CPFPHIG_FAIL == cpfphig_dirname( argv[0],
                                         strlen( argv[0] ) + 1,
                                         bin_dir,
                                         CPFPHIG_BUFFER_SIZE,
                                         NULL ) )
    {
        return CPFPHIG_FAIL;
    }

    // windows will be _splitpath

    const struct CMUnitTest tests[] = {
        cmocka_unit_test(list_at_least_this_binary),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
