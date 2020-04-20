#include "fphig/fphig.h"
#include "fphig/thread.h"
#include "fphig/thread_create.h"
#include "fphig/thread_exit.h"
#include "fphig/thread_cond_init.h"
#include "fphig/thread_cond_attr.h"
#include "fphig/thread_cond_attr_init.h"
#include "fphig/thread_cond_attr_destroy.h"
#include "fphig/thread_cond_destroy.h"
#include "fphig/thread_cond_signal.h"
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

    assert_int_equal( FPHIG_OK, fphig_mutex_lock( &(cond_and_mutex->mutex),
                                                    NULL ) );


    assert_int_equal( FPHIG_OK, fphig_thread_cond_signal( &(cond_and_mutex->cond),
                                                            NULL ) );


    assert_int_equal( FPHIG_OK, fphig_mutex_unlock( &(cond_and_mutex->mutex),
                                                      NULL ) );

    assert_int_equal( FPHIG_OK, fphig_thread_exit( NULL,
                                                     NULL ) );

    assert_non_null( NULL );
    return NULL;
}

static void signal_wait( void** state )
{
    struct fphig_thread         thread              = FPHIG_CONST_FPHIG_THREAD;
    struct fphig_thread_attr    thread_attr         = FPHIG_CONST_FPHIG_THREAD_ATTR;
    struct cond_and_mutex       cond_and_mutex      = {
        FPHIG_CONST_FPHIG_THREAD_COND,
        FPHIG_CONST_FPHIG_THREAD_COND_ATTR,
        FPHIG_CONST_FPHIG_MUTEX,
        FPHIG_CONST_FPHIG_MUTEX_ATTR
    };

    assert_int_equal( FPHIG_OK, fphig_mutex_init( &(cond_and_mutex.mutex),
                                                    &(cond_and_mutex.mutex_attr),
                                                    NULL ) );

    assert_int_equal( FPHIG_OK, fphig_thread_cond_init( &(cond_and_mutex.cond),
                                                          &(cond_and_mutex.cond_attr),
                                                          NULL ) );

    assert_int_equal( FPHIG_OK, fphig_mutex_lock( &(cond_and_mutex.mutex),
                                                    NULL ) );


    assert_int_equal( FPHIG_OK, fphig_thread_create( &thread,
                                                       &thread_attr,
                                                       &start_routine,
                                                       &cond_and_mutex,
                                                       NULL ) );

    assert_int_equal( FPHIG_OK, fphig_thread_cond_wait( &(cond_and_mutex.cond),
                                                          &(cond_and_mutex.mutex),
                                                          NULL ) );


    assert_int_equal( FPHIG_OK, fphig_mutex_unlock( &(cond_and_mutex.mutex),
                                                      NULL ) );

    assert_int_equal( FPHIG_OK, fphig_thread_cond_destroy( &(cond_and_mutex.cond),
                                                             NULL ) );

    assert_int_equal( FPHIG_OK, fphig_mutex_destroy( &(cond_and_mutex.mutex),
                                                       NULL ) );


}

int main( int argc, char* argv[]  )
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(signal_wait),

    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
