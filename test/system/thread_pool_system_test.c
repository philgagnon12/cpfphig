#include "fphig/fphig.h"
#include "fphig/thread_pool.h"
#include "fphig/thread_pool_create.h"
#include "fphig/thread_pool_task.h"
#include "fphig/destroy_thread_pool.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <time.h>
#include <sys/time.h>
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
    struct fphig_thread_pool thread_pool    = FPHIG_CONST_MPHIG_THREAD_POOL;
    struct timespec timestamp               = { 0, 0 };
    struct timespec realtime                = { 0, 0 };

    checked = 0;

    assert_int_equal( FPHIG_OK, fphig_thread_pool_create( &thread_pool,
                                                            NULL ) );


    assert_int_equal( FPHIG_OK, fphig_thread_pool_task( &thread_pool,
                                                          &routine,
                                                          NULL,
                                                          NULL ) );

    assert_int_equal( FPHIG_OK, fphig_thread_pool_task( &thread_pool,
                                                          &routine,
                                                          NULL,
                                                          NULL ) );

    assert_int_equal(0, clock_gettime(CLOCK_REALTIME, &timestamp ) );

    // spin lock for max 2 seconds
    while( checked != 2)
    {
        assert_int_equal(0, clock_gettime(CLOCK_REALTIME, &realtime ) );
        assert_true( ( realtime.tv_sec - timestamp.tv_sec) < 2 );
    }

    assert_int_equal( FPHIG_OK, fphig_destroy_thread_pool( &thread_pool,
                                                             NULL ) );
}

int main( int argc, char* argv[]  )
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(create_task_and_destroy),

    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
