#include "cpfphig/cpfphig.h"
#include "cpfphig/thread.h"
#include "cpfphig/thread_create.h"
#include "cpfphig/thread_join.h"
#include "cpfphig/thread_exit.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <string.h>

static int arg = 0;

static
int
start_routine( void* Arg )
{
    assert_non_null( Arg );
    assert_int_equal( 777, *(int*)Arg );

    cpfphig_thread_exit( 888 );
    assert_non_null( NULL );
    return 0;
}

static void create_exit_join( void** state )
{
    struct cpfphig_thread         thread      = CPFPHIG_CONST_CPFPHIG_THREAD;
    struct cpfphig_thread_attr    thread_attr = CPFPHIG_CONST_CPFPHIG_THREAD_ATTR;
    int                           exit_ret    = 0;

    arg = 777;

    assert_int_equal( CPFPHIG_OK, cpfphig_thread_create( &thread,
                                                         &thread_attr,
                                                         &start_routine,
                                                         &arg,
                                                         NULL ) );

    assert_int_equal( CPFPHIG_OK, cpfphig_thread_join( &thread,
                                                       &exit_ret,
                                                       NULL ) );

    assert_non_null( exit_ret );
    assert_int_equal( 888, exit_ret );
}

int main( int argc, char* argv[]  )
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(create_exit_join),

    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
