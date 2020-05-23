#include "cpfphig/cpfphig.h"
#include "cpfphig/publisher.h"
#include "cpfphig/publisher_create.h"
#include "cpfphig/publisher_subscribe.h"
#include "cpfphig/publisher_publish.h"
#include "cpfphig/publisher_destroy.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <string.h>

static int checked = 0;

static
cpfphig
subscription_routine( void*                                     Publisher_Data,
                      void*                                     Subscription_Data,
                      CPFPHIG_OPTIONAL struct cpfphig_error*    Error )
{
    checked += 1;
    return CPFPHIG_OK;
}



static void create_subscribe_publish_destroy( void** state )
{
    struct cpfphig_publisher      publisher               = CPFPHIG_CONST_CPFPHIG_PUBLISHER;
    struct cpfphig_subscription*  first_subscription      = NULL;
    struct cpfphig_subscription*  second_subscription     = NULL;
    struct cpfphig_subscription*  third_subscription      = NULL;

    assert_int_equal( CPFPHIG_OK, cpfphig_publisher_create( &publisher,
                                                            NULL ) );

    assert_int_equal( CPFPHIG_OK, cpfphig_publisher_subscribe( &publisher,
                                                               NULL,
                                                               &subscription_routine,
                                                               &first_subscription,
                                                               NULL ) );

    assert_int_equal( CPFPHIG_OK, cpfphig_publisher_subscribe( &publisher,
                                                               NULL,
                                                               &subscription_routine,
                                                               &second_subscription,
                                                               NULL ) );

    assert_int_equal( CPFPHIG_OK, cpfphig_publisher_publish( &publisher,
                                                             NULL,
                                                             cpfphig_publisher_thread_cond_kind_data,
                                                             NULL ) );

    // Wont get run
    assert_int_equal( CPFPHIG_OK, cpfphig_publisher_subscribe( &publisher,
                                                               NULL,
                                                               &subscription_routine,
                                                               &third_subscription,
                                                               NULL ) );

    assert_int_equal( CPFPHIG_OK, cpfphig_publisher_destroy( &publisher,
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
