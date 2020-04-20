#include "fphig/fphig.h"
#include "fphig/publisher.h"
#include "fphig/publisher_create.h"
#include "fphig/publisher_subscribe.h"
#include "fphig/publisher_publish.h"
#include "fphig/publisher_destroy.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <string.h>

static int checked = 0;

static
fphig
subscription_routine( void*                                   Publisher_Data,
                      void*                                   Subscription_Data,
                      FPHIG_OPTIONAL struct fphig_error*    Error )
{
    checked += 1;
    return FPHIG_OK;
}



static void create_subscribe_publish_destroy( void** state )
{
    struct fphig_publisher      publisher               = FPHIG_CONST_FPHIG_PUBLISHER;
    struct fphig_subscription*  first_subscription      = NULL;
    struct fphig_subscription*  second_subscription     = NULL;
    struct fphig_subscription*  third_subscription      = NULL;

    assert_int_equal( FPHIG_OK, fphig_publisher_create( &publisher,
                                                          NULL ) );

    assert_int_equal( FPHIG_OK, fphig_publisher_subscribe( &publisher,
                                                             NULL,
                                                             &subscription_routine,
                                                             &first_subscription,
                                                             NULL ) );

    assert_int_equal( FPHIG_OK, fphig_publisher_subscribe( &publisher,
                                                             NULL,
                                                             &subscription_routine,
                                                             &second_subscription,
                                                             NULL ) );

    assert_int_equal( FPHIG_OK, fphig_publisher_publish( &publisher,
                                                           NULL,
                                                           fphig_publisher_thread_cond_kind_data,
                                                           NULL ) );

    // Wont get run
    assert_int_equal( FPHIG_OK, fphig_publisher_subscribe( &publisher,
                                                             NULL,
                                                             &subscription_routine,
                                                             &third_subscription,
                                                             NULL ) );

    assert_int_equal( FPHIG_OK, fphig_publisher_destroy( &publisher,
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
