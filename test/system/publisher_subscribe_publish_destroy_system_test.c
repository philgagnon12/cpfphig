#include "melphig/melphig.h"
#include "melphig/publisher.h"
#include "melphig/publisher_create.h"
#include "melphig/publisher_subscribe.h"
#include "melphig/publisher_publish.h"
#include "melphig/publisher_destroy.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <string.h>

static int checked = 0;

static
mphig
subscription_routine( void*                                   Publisher_Data,
                      void*                                   Subscription_Data,
                      MELPHIG_OPTIONAL struct mphig_error*    Error )
{
    checked += 1;
    return MELPHIG_OK;
}



static void create_subscribe_publish_destroy( void** state )
{
    struct mphig_publisher      publisher               = MELPHIG_CONST_MPHIG_PUBLISHER;
    struct mphig_subscription*  first_subscription      = NULL;
    struct mphig_subscription*  second_subscription     = NULL;
    struct mphig_subscription*  third_subscription      = NULL;

    assert_int_equal( MELPHIG_OK, mphig_publisher_create( &publisher,
                                                          NULL ) );

    assert_int_equal( MELPHIG_OK, mphig_publisher_subscribe( &publisher,
                                                             NULL,
                                                             &subscription_routine,
                                                             &first_subscription,
                                                             NULL ) );

    assert_int_equal( MELPHIG_OK, mphig_publisher_subscribe( &publisher,
                                                             NULL,
                                                             &subscription_routine,
                                                             &second_subscription,
                                                             NULL ) );

    assert_int_equal( MELPHIG_OK, mphig_publisher_publish( &publisher,
                                                           NULL,
                                                           mphig_publisher_thread_cond_kind_data,
                                                           NULL ) );

    // Wont get run
    assert_int_equal( MELPHIG_OK, mphig_publisher_subscribe( &publisher,
                                                             NULL,
                                                             &subscription_routine,
                                                             &third_subscription,
                                                             NULL ) );

    assert_int_equal( MELPHIG_OK, mphig_publisher_destroy( &publisher,
                                                           NULL ) );

    assert_int_equal( 2, checked );
}

int main( int argc, char* argv[]  )
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(create_subscribe_publish_destroy),

    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
