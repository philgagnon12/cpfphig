#include "cpfphig/cpfphig.h"
#include "cpfphig/publisher_destroy.h"
#include "cpfphig/publisher.h"
#include "cpfphig/publisher_unsubscribe.h"
#include "cpfphig/list.h"
#include "cpfphig/free.h"
#include "cpfphig/mutex_destroy.h"
#include "cpfphig/thread_cond_destroy.h"

cpfphig
cpfphig_publisher_destroy( struct cpfphig_publisher*                Publisher,
                         CPFPHIG_OPTIONAL struct cpfphig_error*   Error )
{
    cpfphig                                           ret                 = CPFPHIG_FAIL;
    struct cpfphig_list*                              subscriptions       = NULL;
    struct cpfphig_list                               subscriptions_copy  = CPFPHIG_CONST_CPFPHIG_LIST;
    struct cpfphig_subscription*                      subscription        = NULL;
    cpfphig                                           shift_ret           = CPFPHIG_FAIL;
    struct cpfphig_error                              shift_error         = CPFPHIG_CONST_CPFPHIG_ERROR;

    // NULL checks
    if( Publisher == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Publisher is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    subscriptions = Publisher->subscriptions;

    if( subscriptions == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Publisher->subscriptions is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    if( CPFPHIG_FAIL == cpfphig_list_copy( *subscriptions,
                                         &subscriptions_copy,
                                         Error ) )
    {
        return CPFPHIG_FAIL;
    }

    while( CPFPHIG_OK == ( shift_ret = cpfphig_list_shift( &subscriptions_copy,
                                                         &subscription,
                                                         &shift_error ) ) )
    {
        if( subscription == NULL )
        {
            if( Error != NULL )
                cpfphig_error_message(cpfphig_system_error, "subscription is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

            return CPFPHIG_FAIL;
        }

        if( CPFPHIG_FAIL == cpfphig_publisher_unsubscribe( subscription,
                                                         Error ) )
        {
            return CPFPHIG_FAIL;
        }
    }

    if( shift_ret == CPFPHIG_FAIL &&
        shift_error.error_type == cpfphig_system_error )
    {
        if( Error != NULL )
            *Error = shift_error;

        return CPFPHIG_FAIL;
    }

    if( CPFPHIG_FAIL == cpfphig_free( &subscriptions,
                                    Error ) )
    {
        return CPFPHIG_FAIL;
    }

    if( CPFPHIG_FAIL == cpfphig_mutex_destroy( &(Publisher->completed_ack_mutex),
                                             Error ) )
    {
        return CPFPHIG_FAIL;
    }

    if( CPFPHIG_FAIL == cpfphig_mutex_destroy( &(Publisher->completed_mutex),
                                             Error ) )
    {
        return CPFPHIG_FAIL;
    }

    if( CPFPHIG_FAIL == cpfphig_mutex_destroy( &(Publisher->broadcast_mutex),
                                             Error ) )
    {
        return CPFPHIG_FAIL;
    }

    if( CPFPHIG_FAIL == cpfphig_mutex_destroy( &(Publisher->mutex),
                                             Error ) )
    {
        return CPFPHIG_FAIL;
    }

    if( CPFPHIG_FAIL == cpfphig_thread_cond_destroy( &(Publisher->completed_ack_thread_cond),
                                                   Error ) )
    {
        return CPFPHIG_FAIL;
    }

    if( CPFPHIG_FAIL == cpfphig_thread_cond_destroy( &(Publisher->completed_thread_cond),
                                                   Error ) )
    {
        return CPFPHIG_FAIL;
    }

    if( CPFPHIG_FAIL == cpfphig_thread_cond_destroy( &(Publisher->publish_thread_cond),
                                                   Error ) )
    {
        return CPFPHIG_FAIL;
    }

    return CPFPHIG_OK;
}


