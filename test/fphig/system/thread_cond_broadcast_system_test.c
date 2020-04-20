#include "fphig/fphig.h"
#include "fphig/thread.h"
#include "fphig/thread_create.h"
#include "fphig/thread_exit.h"
#include "fphig/thread_cond_init.h"
#include "fphig/thread_cond_attr.h"
#include "fphig/thread_cond_attr_init.h"
#include "fphig/thread_cond_attr_destroy.h"
#include "fphig/thread_cond_destroy.h"
#include "fphig/thread_cond_broadcast.h"
#include "fphig/thread_cond_wait.h"
#include "fphig/mutex.h"
#include "fphig/mutex_attr.h"
#include "fphig/mutex_init.h"
#include "fphig/mutex_destroy.h"
#include "fphig/mutex_lock.h"
#include "fphig/mutex_unlock.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <string.h>

static struct fphig_mutex       thread_start_mutex      = FPHIG_CONST_FPHIG_MUTEX;
static struct fphig_mutex_attr  thread_start_mutex_attr = FPHIG_CONST_FPHIG_MUTEX_ATTR;
static int                      thread_start_count      = 0;
static int                      broadcasted_count       = 0;

struct cond_and_mutex
{
    struct fphig_thread_cond        cond;
    struct fphig_thread_cond_attr   cond_attr;
    struct fphig_mutex              mutex;
    struct fphig_mutex_attr         mutex_attr;
};

static
void*
start_routine( void* Arg )
{
    struct cond_and_mutex*   cond_and_mutex = NULL;

    assert_non_null( Arg );

    cond_and_mutex = (struct cond_and_mutex*)Arg;

    assert_int_equal( FPHIG_OK, fphig_mutex_lock( &thread_start_mutex,
                                                    NULL ) );

    thread_start_count++;

    assert_int_equal( FPHIG_OK, fphig_mutex_lock( &(cond_and_mutex->mutex),
                                                    NULL ) );

    assert_int_equal( FPHIG_OK, fphig_mutex_unlock( &thread_start_mutex,
                                                      NULL ) );

    assert_int_equal( FPHIG_OK, fphig_thread_cond_wait( &(cond_and_mutex->cond),
                                                          &(cond_and_mutex->mutex),
                                                          NULL ) );

    broadcasted_count++;

    assert_int_equal( FPHIG_OK, fphig_mutex_unlock( &(cond_and_mutex->mutex),
                                                      NULL ) );

    assert_int_equal( FPHIG_OK, fphig_thread_exit( NULL,
                                                     NULL ) );

    assert_non_null( NULL );
    return NULL;
}

static void wait_broadcast( void** state )
{
    struct fphig_thread         first_thread        = FPHIG_CONST_FPHIG_THREAD;
    struct fphig_thread_attr    first_thread_attr   = FPHIG_CONST_FPHIG_THREAD_ATTR;
    struct fphig_thread         second_thread       = FPHIG_CONST_FPHIG_THREAD;
    struct fphig_thread_attr    second_thread_attr  = FPHIG_CONST_FPHIG_THREAD_ATTR;
    struct cond_and_mutex       cond_and_mutex      = {
        FPHIG_CONST_FPHIG_THREAD_COND,
        FPHIG_CONST_FPHIG_THREAD_COND_ATTR,
        FPHIG_CONST_FPHIG_MUTEX,
        FPHIG_CONST_FPHIG_MUTEX_ATTR
    };
    int                         start_count         = 0;
    int                         synchronized_count  = 0;
    int                         count               = 0;

    assert_int_equal( FPHIG_OK, fphig_mutex_init( &thread_start_mutex,
                                                    &thread_start_mutex_attr,
                                                    NULL ) );


    assert_int_equal( FPHIG_OK, fphig_mutex_init( &(cond_and_mutex.mutex),
                                                    &(cond_and_mutex.mutex_attr),
                                                    NULL ) );

    assert_int_equal( FPHIG_OK, fphig_thread_cond_init( &(cond_and_mutex.cond),
                                                          &(cond_and_mutex.cond_attr),
                                                          NULL ) );

    assert_int_equal( FPHIG_OK, fphig_thread_create( &first_thread,
                                                       &first_thread_attr,
                                                       &start_routine,
                                                       &cond_and_mutex,
                                                       NULL ) );

    assert_int_equal( FPHIG_OK, fphig_thread_create( &second_thread,
                                                       &second_thread_attr,
                                                       &start_routine,
                                                       &cond_and_mutex,
                                                       NULL ) );

    // Make sure threads are on wait() before sending broadcast
    while( start_count != 2 )
    {
        assert_int_equal( FPHIG_OK, fphig_mutex_lock( &thread_start_mutex,
                                                        NULL ) );

        start_count = thread_start_count;

        assert_int_equal( FPHIG_OK, fphig_mutex_unlock( &thread_start_mutex,
                                                          NULL ) );
    }

    while( synchronized_count != 2 )
    {
        assert_int_equal( FPHIG_OK, fphig_mutex_lock( &(cond_and_mutex.mutex),
                                                        NULL ) );

        assert_int_equal( FPHIG_OK, fphig_mutex_unlock( &(cond_and_mutex.mutex),
                                                          NULL ) );

        synchronized_count++;
    }

    assert_int_equal( FPHIG_OK, fphig_thread_cond_broadcast( &(cond_and_mutex.cond),
                                                               NULL ) );


    while( count != 2 )
    {

        assert_int_equal( FPHIG_OK, fphig_mutex_lock( &(cond_and_mutex.mutex),
                                                        NULL ) );

        count = broadcasted_count;

        assert_int_equal( FPHIG_OK, fphig_mutex_unlock( &(cond_and_mutex.mutex),
                                                          NULL ) );
    }

    assert_int_equal( FPHIG_OK, fphig_thread_cond_destroy( &(cond_and_mutex.cond),
                                                             NULL ) );

    assert_int_equal( FPHIG_OK, fphig_mutex_destroy( &(cond_and_mutex.mutex),
                                                       NULL ) );

    assert_int_equal( FPHIG_OK, fphig_mutex_destroy( &thread_start_mutex,
                                                       NULL ) );

}

int main( int argc, char* argv[]  )
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(wait_broadcast),

    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
