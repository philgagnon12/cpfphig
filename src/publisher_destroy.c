#include "melphig/melphig.h"
#include "melphig/publisher_destroy.h"
#include "melphig/publisher.h"
#include "melphig/publisher_unsubscribe.h"
#include "melphig/list.h"
#include "melphig/free.h"
#include "melphig/mutex_destroy.h"
#include "melphig/thread_cond_destroy.h"

mphig
mphig_publisher_destroy( struct mphig_publisher*                Publisher,
                         MELPHIG_OPTIONAL struct mphig_error*   Error )
{
    mphig                                           ret                 = MELPHIG_FAIL;
    struct mphig_list*                              subscriptions       = NULL;
    struct mphig_list                               subscriptions_copy  = MELPHIG_CONST_MPHIG_LIST;
    struct mphig_subscription*                      subscription        = NULL;
    mphig                                           shift_ret           = MELPHIG_FAIL;
    struct mphig_error                              shift_error         = MELPHIG_CONST_MPHIG_ERROR;

    // NULL checks
    if( Publisher == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Publisher is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    subscriptions = Publisher->subscriptions;

    if( subscriptions == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Publisher->subscriptions is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    if( MELPHIG_FAIL == mphig_list_copy( *subscriptions,
                                         &subscriptions_copy,
                                         Error ) )
    {
        return MELPHIG_FAIL;
    }

    while( MELPHIG_OK == ( shift_ret = mphig_list_shift( &subscriptions_copy,
                                                         &subscription,
                                                         &shift_error ) ) )
    {
        if( subscription == NULL )
        {
            if( Error != NULL )
                mphig_error_message(mphig_system_error, "subscription is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

            return MELPHIG_FAIL;
        }

        if( MELPHIG_FAIL == mphig_publisher_unsubscribe( subscription,
                                                         Error ) )
        {
            return MELPHIG_FAIL;
        }
    }

    if( shift_ret == MELPHIG_FAIL &&
        shift_error.error_type == mphig_system_error )
    {
        if( Error != NULL )
            *Error = shift_error;

        return MELPHIG_FAIL;
    }

    if( MELPHIG_FAIL == mphig_free( &subscriptions,
                                    Error ) )
    {
        return MELPHIG_FAIL;
    }

    if( MELPHIG_FAIL == mphig_mutex_destroy( &(Publisher->completed_ack_mutex),
                                             Error ) )
    {
        return MELPHIG_FAIL;
    }

    if( MELPHIG_FAIL == mphig_mutex_destroy( &(Publisher->completed_mutex),
                                             Error ) )
    {
        return MELPHIG_FAIL;
    }

    if( MELPHIG_FAIL == mphig_mutex_destroy( &(Publisher->broadcast_mutex),
                                             Error ) )
    {
        return MELPHIG_FAIL;
    }

    if( MELPHIG_FAIL == mphig_mutex_destroy( &(Publisher->mutex),
                                             Error ) )
    {
        return MELPHIG_FAIL;
    }

    if( MELPHIG_FAIL == mphig_thread_cond_destroy( &(Publisher->completed_ack_thread_cond),
                                                   Error ) )
    {
        return MELPHIG_FAIL;
    }

    if( MELPHIG_FAIL == mphig_thread_cond_destroy( &(Publisher->completed_thread_cond),
                                                   Error ) )
    {
        return MELPHIG_FAIL;
    }

    if( MELPHIG_FAIL == mphig_thread_cond_destroy( &(Publisher->publish_thread_cond),
                                                   Error ) )
    {
        return MELPHIG_FAIL;
    }

    return MELPHIG_OK;
}


