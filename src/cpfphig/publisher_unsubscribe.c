#include "cpfphig/cpfphig.h"
#include "cpfphig/publisher_unsubscribe.h"
#include "cpfphig/publisher.h"
#include "cpfphig/subscription.h"
#include "cpfphig/publisher_publish.h"
#include "cpfphig/mutex_lock.h"
#include "cpfphig/mutex_unlock.h"
#include "cpfphig/thread_join.h"
#include "cpfphig/free.h"

cpfphig
cpfphig_publisher_unsubscribe( struct cpfphig_subscription*             Subscription,
                             CPFPHIG_OPTIONAL struct cpfphig_error*   Error )
{
    struct cpfphig_publisher*     publisher   = NULL;

    // NULL checks
    if( Subscription == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Subscription is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    publisher = Subscription->publisher;

    if( publisher == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Subscription->publisher is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    if( CPFPHIG_FAIL == cpfphig_mutex_lock( &(publisher->mutex),
                                          Error ) )
    {
        return CPFPHIG_FAIL;
    }

    publisher->unsubscribing_subscription = Subscription;

    if( CPFPHIG_FAIL == cpfphig_mutex_unlock( &(publisher->mutex),
                                            Error ) )
    {
        return CPFPHIG_FAIL;
    }

    if( CPFPHIG_FAIL == cpfphig_publisher_publish( publisher,
                                                 NULL,
                                                 cpfphig_publisher_thread_cond_kind_abort,
                                                 Error ) )
    {
        return CPFPHIG_FAIL;
    }

    if( CPFPHIG_FAIL == cpfphig_thread_join( &(Subscription->thread),
                                           NULL,
                                           Error ) )
    {
        return CPFPHIG_FAIL;
    }


    if( CPFPHIG_FAIL == cpfphig_free( &Subscription,
                                      Error ) )
    {
        return CPFPHIG_FAIL;
    }

    return CPFPHIG_OK;
}


