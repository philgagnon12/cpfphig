#include "melphig/melphig.h"
#include "melphig/thread.h"
#include "melphig/thread_create.h"
#include "melphig/thread_join.h"
#include "melphig/thread_exit.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <string.h>

static int start_routine_ret    = 888;

static
void*
start_routine( void* Arg )
{
    assert_non_null( Arg );
    assert_int_equal( 777, *(int*)Arg );

    assert_int_equal( MELPHIG_OK, mphig_thread_exit( &start_routine_ret,
                                                     NULL ) );
    assert_non_null( NULL );
    return NULL;
}

static void create_exit_join( void** state )
{
    struct mphig_thread         thread      = MELPHIG_CONST_MPHIG_THREAD;
    struct mphig_thread_attr    thread_attr = MELPHIG_CONST_MPHIG_THREAD_ATTR;
    int                         arg         = 777;
    int*                        exit_ret    = NULL;

    assert_int_equal( MELPHIG_OK, mphig_thread_create( &thread,
                                                       &thread_attr,
                                                       &start_routine,
                                                       &arg,
                                                       NULL ) );

    assert_int_equal( MELPHIG_OK, mphig_thread_join( &thread,
                                                     (void**)&exit_ret,
                                                     NULL ) );

    assert_non_null( exit_ret );
    assert_int_equal( 888, *exit_ret );
}

int main( int argc, char* argv[]  )
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(create_exit_join),

    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
