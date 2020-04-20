#include "melphig/melphig.h"
#include "melphig/defer.h"
#include "melphig/thread_pool.h"
#include "melphig/thread_pool_create.h"
#include "melphig/destroy_thread_pool.h"
#include "melphig/sleep.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

static int checked = 0;

static
void
routine( void* Arg )
{
    checked += 1;
}

static void defer( void** state )
{
    struct mphig_thread_pool thread_pool    = MELPHIG_CONST_MPHIG_THREAD_POOL;

    checked = 0;

    assert_int_equal( MELPHIG_OK, mphig_thread_pool_create( &thread_pool,
                                                            NULL ) );


    assert_int_equal( MELPHIG_OK, mphig_defer( &routine,
                                               NULL,
                                               1000,
                                               &thread_pool,
                                               NULL ) );

    // Dont destroy too soon
    assert_int_equal( MELPHIG_OK, mphig_sleep( 1100, NULL ) );
    assert_int_equal( 1, checked );

    assert_int_equal( MELPHIG_OK, mphig_destroy_thread_pool( &thread_pool,
                                                             NULL ) );
}

int main( int argc, char* argv[]  )
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(defer),

    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
