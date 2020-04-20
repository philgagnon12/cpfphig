#include "melphig/melphig.h"
#include "melphig/publisher_destroy.h"
#include "melphig/publisher.h"
#include "melphig/publisher_unsubscribe.h"
#include "melphig/list.h"
#include "melphig/free.h"
#include "melphig/mutex_destroy.h"
#include "melphig/thread_cond_destroy.h"

fphig
fphig_publisher_destroy( struct fphig_publisher*                Publisher,
                         FPHIG_OPTIONAL struct fphig_error*   Error )
{
    fphig                                           ret                 = FPHIG_FAIL;
    struct fphig_list*                              subscriptions       = NULL;
    struct fphig_list                               subscriptions_copy  = FPHIG_CONST_MPHIG_LIST;
    struct fphig_subscription*                      subscription        = NULL;
    fphig                                           shift_ret           = FPHIG_FAIL;
    struct fphig_error                              shift_error         = FPHIG_CONST_MPHIG_ERROR;

    // NULL checks
    if( Publisher == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "Publisher is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    subscriptions = Publisher->subscriptions;

    if( subscriptions == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "Publisher->subscriptions is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    if( FPHIG_FAIL == fphig_list_copy( *subscriptions,
                                         &subscriptions_copy,
                                         Error ) )
    {
        return FPHIG_FAIL;
    }

    while( FPHIG_OK == ( shift_ret = fphig_list_shift( &subscriptions_copy,
                                                         &subscription,
                                                         &shift_error ) ) )
    {
        if( subscription == NULL )
        {
            if( Error != NULL )
                fphig_error_message(fphig_system_error, "subscription is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

            return FPHIG_FAIL;
        }

        if( FPHIG_FAIL == fphig_publisher_unsubscribe( subscription,
                                                         Error ) )
        {
            return FPHIG_FAIL;
        }
    }

    if( shift_ret == FPHIG_FAIL &&
        shift_error.error_type == fphig_system_error )
    {
        if( Error != NULL )
            *Error = shift_error;

        return FPHIG_FAIL;
    }

    if( FPHIG_FAIL == fphig_free( &subscriptions,
                                    Error ) )
    {
        return FPHIG_FAIL;
    }

    if( FPHIG_FAIL == fphig_mutex_destroy( &(Publisher->completed_ack_mutex),
                                             Error ) )
    {
        return FPHIG_FAIL;
    }

    if( FPHIG_FAIL == fphig_mutex_destroy( &(Publisher->completed_mutex),
                                             Error ) )
    {
        return FPHIG_FAIL;
    }

    if( FPHIG_FAIL == fphig_mutex_destroy( &(Publisher->broadcast_mutex),
                                             Error ) )
    {
        return FPHIG_FAIL;
    }

    if( FPHIG_FAIL == fphig_mutex_destroy( &(Publisher->mutex),
                                             Error ) )
    {
        return FPHIG_FAIL;
    }

    if( FPHIG_FAIL == fphig_thread_cond_destroy( &(Publisher->completed_ack_thread_cond),
                                                   Error ) )
    {
        return FPHIG_FAIL;
    }

    if( FPHIG_FAIL == fphig_thread_cond_destroy( &(Publisher->completed_thread_cond),
                                                   Error ) )
    {
        return FPHIG_FAIL;
    }

    if( FPHIG_FAIL == fphig_thread_cond_destroy( &(Publisher->publish_thread_cond),
                                                   Error ) )
    {
        return FPHIG_FAIL;
    }

    return FPHIG_OK;
}


