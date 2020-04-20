#include "melphig/melphig.h"
#include "melphig/thread.h"
#include "melphig/thread_create.h"
#include "melphig/thread_exit.h"
#include "melphig/thread_cond_init.h"
#include "melphig/thread_cond_attr.h"
#include "melphig/thread_cond_attr_init.h"
#include "melphig/thread_cond_attr_destroy.h"
#include "melphig/thread_cond_destroy.h"
#include "melphig/thread_cond_broadcast.h"
#include "melphig/thread_cond_wait.h"
#include "melphig/mutex.h"
#include "melphig/mutex_attr.h"
#include "melphig/mutex_init.h"
#include "melphig/mutex_destroy.h"
#include "melphig/mutex_lock.h"
#include "melphig/mutex_unlock.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <string.h>

static struct mphig_mutex       thread_start_mutex      = MELPHIG_CONST_MPHIG_MUTEX;
static struct mphig_mutex_attr  thread_start_mutex_attr = MELPHIG_CONST_MPHIG_MUTEX_ATTR;
static int                      thread_start_count      = 0;
static int                      broadcasted_count       = 0;

struct cond_and_mutex
{
    struct mphig_thread_cond        cond;
    struct mphig_thread_cond_attr   cond_attr;
    struct mphig_mutex              mutex;
    struct mphig_mutex_attr         mutex_attr;
};

static
void*
start_routine( void* Arg )
{
    struct cond_and_mutex*   cond_and_mutex = NULL;

    assert_non_null( Arg );

    cond_and_mutex = (struct cond_and_mutex*)Arg;

    assert_int_equal( MELPHIG_OK, mphig_mutex_lock( &thread_start_mutex,
                                                    NULL ) );

    thread_start_count++;

    assert_int_equal( MELPHIG_OK, mphig_mutex_lock( &(cond_and_mutex->mutex),
                                                    NULL ) );

    assert_int_equal( MELPHIG_OK, mphig_mutex_unlock( &thread_start_mutex,
                                                      NULL ) );

    assert_int_equal( MELPHIG_OK, mphig_thread_cond_wait( &(cond_and_mutex->cond),
                                                          &(cond_and_mutex->mutex),
                                                          NULL ) );

    broadcasted_count++;

    assert_int_equal( MELPHIG_OK, mphig_mutex_unlock( &(cond_and_mutex->mutex),
                                                      NULL ) );

    assert_int_equal( MELPHIG_OK, mphig_thread_exit( NULL,
                                                     NULL ) );

    assert_non_null( NULL );
    return NULL;
}

static void wait_broadcast( void** state )
{
    struct mphig_thread         first_thread        = MELPHIG_CONST_MPHIG_THREAD;
    struct mphig_thread_attr    first_thread_attr   = MELPHIG_CONST_MPHIG_THREAD_ATTR;
    struct mphig_thread         second_thread       = MELPHIG_CONST_MPHIG_THREAD;
    struct mphig_thread_attr    second_thread_attr  = MELPHIG_CONST_MPHIG_THREAD_ATTR;
    struct cond_and_mutex       cond_and_mutex      = {
        MELPHIG_CONST_MPHIG_THREAD_COND,
        MELPHIG_CONST_MPHIG_THREAD_COND_ATTR,
        MELPHIG_CONST_MPHIG_MUTEX,
        MELPHIG_CONST_MPHIG_MUTEX_ATTR
    };
    int                         start_count         = 0;
    int                         synchronized_count  = 0;
    int                         count               = 0;

    assert_int_equal( MELPHIG_OK, mphig_mutex_init( &thread_start_mutex,
                                                    &thread_start_mutex_attr,
                                                    NULL ) );


    assert_int_equal( MELPHIG_OK, mphig_mutex_init( &(cond_and_mutex.mutex),
                                                    &(cond_and_mutex.mutex_attr),
                                                    NULL ) );

    assert_int_equal( MELPHIG_OK, mphig_thread_cond_init( &(cond_and_mutex.cond),
                                                          &(cond_and_mutex.cond_attr),
                                                          NULL ) );

    assert_int_equal( MELPHIG_OK, mphig_thread_create( &first_thread,
                                                       &first_thread_attr,
                                                       &start_routine,
                                                       &cond_and_mutex,
                                                       NULL ) );

    assert_int_equal( MELPHIG_OK, mphig_thread_create( &second_thread,
                                                       &second_thread_attr,
                                                       &start_routine,
                                                       &cond_and_mutex,
                                                       NULL ) );

    // Make sure threads are on wait() before sending broadcast
    while( start_count != 2 )
    {
        assert_int_equal( MELPHIG_OK, mphig_mutex_lock( &thread_start_mutex,
                                                        NULL ) );

        start_count = thread_start_count;

        assert_int_equal( MELPHIG_OK, mphig_mutex_unlock( &thread_start_mutex,
                                                          NULL ) );
    }

    while( synchronized_count != 2 )
    {
        assert_int_equal( MELPHIG_OK, mphig_mutex_lock( &(cond_and_mutex.mutex),
                                                        NULL ) );

        assert_int_equal( MELPHIG_OK, mphig_mutex_unlock( &(cond_and_mutex.mutex),
                                                          NULL ) );

        synchronized_count++;
    }

    assert_int_equal( MELPHIG_OK, mphig_thread_cond_broadcast( &(cond_and_mutex.cond),
                                                               NULL ) );


    while( count != 2 )
    {

        assert_int_equal( MELPHIG_OK, mphig_mutex_lock( &(cond_and_mutex.mutex),
                                                        NULL ) );

        count = broadcasted_count;

        assert_int_equal( MELPHIG_OK, mphig_mutex_unlock( &(cond_and_mutex.mutex),
                                                          NULL ) );
    }

    assert_int_equal( MELPHIG_OK, mphig_thread_cond_destroy( &(cond_and_mutex.cond),
                                                             NULL ) );

    assert_int_equal( MELPHIG_OK, mphig_mutex_destroy( &(cond_and_mutex.mutex),
                                                       NULL ) );

    assert_int_equal( MELPHIG_OK, mphig_mutex_destroy( &thread_start_mutex,
                                                       NULL ) );

}

int main( int argc, char* argv[]  )
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(wait_broadcast),

    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
