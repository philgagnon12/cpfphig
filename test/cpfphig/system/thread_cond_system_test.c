#include "cpfphig/cpfphig.h"
#include "cpfphig/thread.h"
#include "cpfphig/thread_create.h"
#include "cpfphig/thread_exit.h"
#include "cpfphig/thread_cond_init.h"
#include "cpfphig/thread_cond_attr.h"
#include "cpfphig/thread_cond_attr_init.h"
#include "cpfphig/thread_cond_attr_destroy.h"
#include "cpfphig/thread_cond_destroy.h"
#include "cpfphig/thread_cond_signal.h"
#include "cpfphig/thread_cond_wait.h"
#include "cpfphig/mutex.h"
#include "cpfphig/mutex_attr.h"
#include "cpfphig/mutex_init.h"
#include "cpfphig/mutex_destroy.h"
#include "cpfphig/mutex_lock.h"
#include "cpfphig/mutex_unlock.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <string.h>

struct cond_and_mutex
{
    struct cpfphig_thread_cond        cond;
    struct cpfphig_thread_cond_attr   cond_attr;
    struct cpfphig_mutex              mutex;
    struct cpfphig_mutex_attr         mutex_attr;
};

static
int
start_routine( void* Arg )
{
    struct cond_and_mutex*   cond_and_mutex = NULL;

    assert_non_null( Arg );

    cond_and_mutex = (struct cond_and_mutex*)Arg;

    assert_int_equal( CPFPHIG_OK, cpfphig_mutex_lock( &(cond_and_mutex->mutex),
                                                    NULL ) );


    assert_int_equal( CPFPHIG_OK, cpfphig_thread_cond_signal( &(cond_and_mutex->cond),
                                                            NULL ) );


    assert_int_equal( CPFPHIG_OK, cpfphig_mutex_unlock( &(cond_and_mutex->mutex),
                                                      NULL ) );

    cpfphig_thread_exit( 0 );

    assert_non_null( NULL );
    return 0;
}

static void signal_wait( void** state )
{
    struct cpfphig_thread         thread              = CPFPHIG_CONST_CPFPHIG_THREAD;
    struct cpfphig_thread_attr    thread_attr         = CPFPHIG_CONST_CPFPHIG_THREAD_ATTR;
    struct cond_and_mutex       cond_and_mutex      = {
        CPFPHIG_CONST_CPFPHIG_THREAD_COND,
        CPFPHIG_CONST_CPFPHIG_THREAD_COND_ATTR,
        CPFPHIG_CONST_CPFPHIG_MUTEX,
        CPFPHIG_CONST_CPFPHIG_MUTEX_ATTR
    };

    assert_int_equal( CPFPHIG_OK, cpfphig_mutex_init( &(cond_and_mutex.mutex),
                                                    &(cond_and_mutex.mutex_attr),
                                                    NULL ) );

    assert_int_equal( CPFPHIG_OK, cpfphig_thread_cond_init( &(cond_and_mutex.cond),
                                                          &(cond_and_mutex.cond_attr),
                                                          NULL ) );

    assert_int_equal( CPFPHIG_OK, cpfphig_mutex_lock( &(cond_and_mutex.mutex),
                                                    NULL ) );


    assert_int_equal( CPFPHIG_OK, cpfphig_thread_create( &thread,
                                                       &thread_attr,
                                                       &start_routine,
                                                       &cond_and_mutex,
                                                       NULL ) );

    assert_int_equal( CPFPHIG_OK, cpfphig_thread_cond_wait( &(cond_and_mutex.cond),
                                                          &(cond_and_mutex.mutex),
                                                          NULL ) );


    assert_int_equal( CPFPHIG_OK, cpfphig_mutex_unlock( &(cond_and_mutex.mutex),
                                                      NULL ) );

    assert_int_equal( CPFPHIG_OK, cpfphig_thread_cond_destroy( &(cond_and_mutex.cond),
                                                             NULL ) );

    assert_int_equal( CPFPHIG_OK, cpfphig_mutex_destroy( &(cond_and_mutex.mutex),
                                                       NULL ) );


}

int main( int argc, char* argv[]  )
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(signal_wait),

    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
