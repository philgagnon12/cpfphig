#include "melphig/melphig.h"
#include "melphig/publisher_unsubscribe.h"
#include "melphig/publisher.h"
#include "melphig/subscription.h"
#include "melphig/publisher_publish.h"
#include "melphig/mutex_lock.h"
#include "melphig/mutex_unlock.h"
#include "melphig/thread_join.h"
#include "melphig/free.h"

mphig
mphig_publisher_unsubscribe( struct mphig_subscription*             Subscription,
                             MELPHIG_OPTIONAL struct mphig_error*   Error )
{
    struct mphig_publisher*     publisher   = NULL;

    // NULL checks
    if( Subscription == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Subscription is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    publisher = Subscription->publisher;

    if( publisher == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Subscription->publisher is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    if( MELPHIG_FAIL == mphig_mutex_lock( &(publisher->mutex),
                                          Error ) )
    {
        return MELPHIG_FAIL;
    }

    publisher->unsubscribing_subscription = Subscription;

    if( MELPHIG_FAIL == mphig_mutex_unlock( &(publisher->mutex),
                                            Error ) )
    {
        return MELPHIG_FAIL;
    }

    if( MELPHIG_FAIL == mphig_publisher_publish( publisher,
                                                 NULL,
                                                 mphig_publisher_thread_cond_kind_abort,
                                                 Error ) )
    {
        return MELPHIG_FAIL;
    }

    if( MELPHIG_FAIL == mphig_thread_join( &(Subscription->thread),
                                           NULL,
                                           Error ) )
    {
        return MELPHIG_FAIL;
    }


    if( MELPHIG_FAIL == mphig_free( Subscription,
                                    Error ) )
    {
        return MELPHIG_FAIL;
    }

    return MELPHIG_OK;
}


