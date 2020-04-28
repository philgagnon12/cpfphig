#include "cpfphig/cpfphig.h"
#include "cpfphig/thread_pool.h"
#include "cpfphig/thread_pool_create.h"
#include "cpfphig/thread_pool_task.h"
#include "cpfphig/destroy_thread_pool.h"
#include "cpfphig/sleep.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <string.h>

static int checked = 0;

static
void
routine( void* Arg )
{
    checked += 1;
}

static void create_task_and_destroy( void** state )
{
    struct cpfphig_thread_pool thread_pool    = CPFPHIG_CONST_CPFPHIG_THREAD_POOL;
    int                        clock          = 0;

    checked = 0;

    assert_int_equal( CPFPHIG_OK, cpfphig_thread_pool_create( &thread_pool,
                                                              NULL ) );


    assert_int_equal( CPFPHIG_OK, cpfphig_thread_pool_task( &thread_pool,
                                                            &routine,
                                                            NULL,
                                                            NULL ) );

    assert_int_equal( CPFPHIG_OK, cpfphig_thread_pool_task( &thread_pool,
                                                            &routine,
                                                            NULL,
                                                            NULL ) );


    // spin lock for max 2 seconds
    while( checked != 2)
    {
        assert_true( cpfphig_sleep( 10, NULL ) );
        assert_true( clock < 200 );
        clock++;
    }

    assert_int_equal( CPFPHIG_OK, cpfphig_destroy_thread_pool( &thread_pool,
                                                               NULL ) );
}

int main( int argc, char* argv[]  )
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(create_task_and_destroy),

    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
