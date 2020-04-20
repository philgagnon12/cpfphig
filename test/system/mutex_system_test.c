#include "melphig/melphig.h"
#include "melphig/mutex.h"
#include "melphig/mutex_init.h"
#include "melphig/mutex_lock.h"
#include "melphig/mutex_unlock.h"
#include "melphig/mutex_destroy.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

static void init_lock_unlock_destroy( void** state )
{
    struct mphig_mutex          mutex       = MELPHIG_CONST_MPHIG_MUTEX;
    struct mphig_mutex_attr     mutex_attr  = MELPHIG_CONST_MPHIG_MUTEX_ATTR;

    assert_int_equal( MELPHIG_OK, mphig_mutex_init( &mutex,
                                                    &mutex_attr,
                                                    NULL ) );

    assert_int_equal( MELPHIG_OK, mphig_mutex_lock( &mutex,
                                                    NULL ) );

    assert_int_equal( MELPHIG_OK, mphig_mutex_unlock( &mutex,
                                                      NULL ) );

    assert_int_equal( MELPHIG_OK, mphig_mutex_destroy( &mutex,
                                                       NULL ) );

}

int main( int argc, char* argv[]  )
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(init_lock_unlock_destroy),

    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
