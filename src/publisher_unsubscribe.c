#include "melphig/melphig.h"
#include "melphig/publisher_unsubscribe.h"
#include "melphig/publisher.h"
#include "melphig/subscription.h"
#include "melphig/publisher_publish.h"
#include "melphig/mutex_lock.h"
#include "melphig/mutex_unlock.h"
#include "melphig/thread_join.h"
#include "melphig/free.h"

fphig
fphig_publisher_unsubscribe( struct fphig_subscription*             Subscription,
                             FPHIG_OPTIONAL struct fphig_error*   Error )
{
    struct fphig_publisher*     publisher   = NULL;

    // NULL checks
    if( Subscription == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "Subscription is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    publisher = Subscription->publisher;

    if( publisher == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "Subscription->publisher is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    if( FPHIG_FAIL == fphig_mutex_lock( &(publisher->mutex),
                                          Error ) )
    {
        return FPHIG_FAIL;
    }

    publisher->unsubscribing_subscription = Subscription;

    if( FPHIG_FAIL == fphig_mutex_unlock( &(publisher->mutex),
                                            Error ) )
    {
        return FPHIG_FAIL;
    }

    if( FPHIG_FAIL == fphig_publisher_publish( publisher,
                                                 NULL,
                                                 fphig_publisher_thread_cond_kind_abort,
                                                 Error ) )
    {
        return FPHIG_FAIL;
    }

    if( FPHIG_FAIL == fphig_thread_join( &(Subscription->thread),
                                           NULL,
                                           Error ) )
    {
        return FPHIG_FAIL;
    }


    if( FPHIG_FAIL == fphig_free( Subscription,
                                    Error ) )
    {
        return FPHIG_FAIL;
    }

    return FPHIG_OK;
}


