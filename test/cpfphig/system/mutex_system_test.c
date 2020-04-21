#include "cpfphig/cpfphig.h"
#include "cpfphig/mutex.h"
#include "cpfphig/mutex_init.h"
#include "cpfphig/mutex_lock.h"
#include "cpfphig/mutex_unlock.h"
#include "cpfphig/mutex_destroy.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

static void init_lock_unlock_destroy( void** state )
{
    struct cpfphig_mutex          mutex       = CPFPHIG_CONST_CPFPHIG_MUTEX;
    struct cpfphig_mutex_attr     mutex_attr  = CPFPHIG_CONST_CPFPHIG_MUTEX_ATTR;

    assert_int_equal( CPFPHIG_OK, cpfphig_mutex_init( &mutex,
                                                    &mutex_attr,
                                                    NULL ) );

    assert_int_equal( CPFPHIG_OK, cpfphig_mutex_lock( &mutex,
                                                    NULL ) );

    assert_int_equal( CPFPHIG_OK, cpfphig_mutex_unlock( &mutex,
                                                      NULL ) );

    assert_int_equal( CPFPHIG_OK, cpfphig_mutex_destroy( &mutex,
                                                       NULL ) );

}

int main( int argc, char* argv[]  )
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(init_lock_unlock_destroy),

    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
