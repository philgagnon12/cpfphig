#include "cpfphig/cpfphig.h"
#include "cpfphig/publisher_subscribe.h"
#include "cpfphig/publisher.h"
#include "cpfphig/subscription.h"
#include "cpfphig/assert.h"
#include "cpfphig/thread_attr.h"
#include "cpfphig/malloc.h"
#include "cpfphig/mutex_lock.h"
#include "cpfphig/thread_create.h"
#include "cpfphig/mutex_unlock.h"
#include "cpfphig/free.h"
#include "cpfphig/list.h"
#include "cpfphig/thread.h"
#include "cpfphig/thread_cond_wait.h"
#include "cpfphig/thread_cond_signal.h"
#include "cpfphig/thread_cond_timed_wait.h"
#include "cpfphig/thread_cond_init.h"
#include "cpfphig/thread_cond_destroy.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

#define CPFPHIG_BUFFER_SIZE ( 0xFF )

static
int
cpfphig_publisher_subscription_routine( void* Subscription )
{
    struct cpfphig_subscription*                      subscription                = NULL;
    struct cpfphig_publisher*                         publisher                   = NULL;
    cpfphig                                           ret                         = CPFPHIG_FAIL;
    int                                               abort                       = 0;
    struct cpfphig_error*                             error                       = NULL;
    struct cpfphig_error                              const_error                 = CPFPHIG_CONST_CPFPHIG_ERROR;
    enum cpfphig_publisher_thread_cond_kind           published_thread_cond_kind  = cpfphig_publisher_thread_cond_kind_abort;
    cpfphig                                           ret_to_signal               = CPFPHIG_FAIL;

    char    error_message_buffer[CPFPHIG_BUFFER_SIZE];

    // When Subscription is null we have no way to signal an error, abort process
    subscription = (struct cpfphig_subscription*)Subscription;
    cpfphig_assert( subscription != NULL, "Subscription is NULL", __FILE__, __FUNCTION__, __LINE__ );


    // When publisher is null we have no way to signal an error, abort process
    publisher = subscription->publisher;
    cpfphig_assert( publisher != NULL, "subscription->publisher is NULL", __FILE__, __FUNCTION__, __LINE__ );

    cpfphig_assert( CPFPHIG_OK == cpfphig_malloc( sizeof( struct cpfphig_error ),
                                              &error,
                                              NULL ),
                  "cpfphig_malloc failed",
                  __FILE__,
                  __FUNCTION__,
                  __LINE__ );

    *error = const_error;

    cpfphig_assert( CPFPHIG_OK == cpfphig_mutex_lock( &(publisher->broadcast_mutex),
                                                  NULL ),
                  "cpfphig_mutex_lock failed",
                  __FILE__,
                  __FUNCTION__,
                  __LINE__ );

    // Assume ok
    ret = CPFPHIG_OK;

    while( abort == 0 )
    {
        // unlocked when wait is called
        if( CPFPHIG_OK == ( ret = cpfphig_thread_cond_wait( &(publisher->publish_thread_cond),
                                                          &(publisher->broadcast_mutex),
                                                          error ) ) )
        {

            if( subscription->thread_ready == 0 )
            {
                subscription->thread_ready = 1;
                // TODO is sending the signal once good enough ?
                cpfphig_assert( CPFPHIG_OK == cpfphig_thread_cond_signal( &subscription->ready_thread_cond,
                                                                      NULL ),
                      "cpfphig_mutex_unlock failed",
                      __FILE__,
                      __FUNCTION__,
                      __LINE__ );
                continue;
            }
            // When publisher is null we have no way to signal an error, abort process
            publisher = subscription->publisher;
            cpfphig_assert( publisher != NULL, "subscription->publisher is NULL", __FILE__, __FUNCTION__, __LINE__ );

            switch( publisher->published_thread_cond_kind )
            {
                case cpfphig_publisher_thread_cond_kind_ready:
                    // Ignore, most likely not intended recipient
                    continue;
                    break;
                case cpfphig_publisher_thread_cond_kind_abort:

                    // Yes below is safe we have the general mutex

                    // ret is OK
                    ret_to_signal = CPFPHIG_OK;

                    if( subscription == publisher->unsubscribing_subscription)
                    {
                        abort = 1;

                        // remove itself from scheduler subscriptions
                        ret_to_signal = cpfphig_list_remove( publisher->subscriptions,
                                                           subscription,
                                                           error );

                        publisher->unsubscribing_subscription = NULL;
                    }

                    if( ret_to_signal == CPFPHIG_OK )
                    {
                        // Report as complete even if the abort was not meant for this subscription
                        publisher->completed_thread_cond_kind = cpfphig_publisher_thread_cond_kind_completed;
                    }
                    break;
                case cpfphig_publisher_thread_cond_kind_data:

                    cpfphig_assert( subscription->subscription_routine != NULL, "subscription->subscription_routine is NULL", __FILE__, __FUNCTION__, __LINE__ );

                    // Unlock broadcast mutex to allow subscription routines to run in parrallel
                    cpfphig_assert( CPFPHIG_OK == cpfphig_mutex_unlock( &(publisher->broadcast_mutex),
                                                                NULL ),
                              "cpfphig_mutex_unlock failed",
                              __FILE__,
                              __FUNCTION__,
                              __LINE__ );

                    ret_to_signal = subscription->subscription_routine( publisher->data,
                                                                        subscription->data,
                                                                        error );

                    cpfphig_assert( CPFPHIG_OK == cpfphig_mutex_lock( &(publisher->broadcast_mutex),
                                                                NULL ),
                              "cpfphig_mutex_unlock failed",
                              __FILE__,
                              __FUNCTION__,
                              __LINE__ );

                    break;
                case cpfphig_publisher_thread_cond_kind_completed:
                default:
                    memset( error_message_buffer,
                            0x00,
                            CPFPHIG_BUFFER_SIZE );

                    snprintf( error_message_buffer,
                              CPFPHIG_BUFFER_SIZE,
                              "Unsupported published_thread_cond_kind %02X",
                              published_thread_cond_kind );

                    cpfphig_error_message(cpfphig_system_error, error_message_buffer, error, __FILE__, __FUNCTION__, __LINE__ );

                    ret = CPFPHIG_FAIL;
            } // switch

            if( ret == CPFPHIG_OK )
            {
                if( ret_to_signal == CPFPHIG_FAIL )
                {
                    abort = 1;
                    publisher->published_error            = *error;
                    publisher->completed_thread_cond_kind = cpfphig_publisher_thread_cond_kind_abort;
                }
                else
                {
                    publisher->completed_thread_cond_kind = cpfphig_publisher_thread_cond_kind_completed;
                }

                // Only a single subscriber can receive a complete ack from the publisher
                cpfphig_assert( CPFPHIG_OK == cpfphig_mutex_lock( &(publisher->completed_ack_mutex),
                                                              NULL ),
                              "cpfphig_mutex_lock failed",
                              __FILE__,
                              __FUNCTION__,
                              __LINE__ );


                // Lock to be able to receive our completed ack signal from publisher
                cpfphig_assert( CPFPHIG_OK == cpfphig_mutex_lock( &(publisher->completed_mutex),
                                                              NULL ),
                              "cpfphig_mutex_lock failed",
                              __FILE__,
                              __FUNCTION__,
                              __LINE__ );

                // Send complete signal
                cpfphig_assert( CPFPHIG_OK == cpfphig_thread_cond_signal( &(publisher->completed_thread_cond),
                                                                      NULL ),
                              "cpfphig_thread_cond_signal failed",
                              __FILE__,
                              __FUNCTION__,
                              __LINE__ );

                // wait unlocks the mutex and blocks
                cpfphig_assert( CPFPHIG_OK == cpfphig_thread_cond_wait( &(publisher->completed_ack_thread_cond),
                                                                    &(publisher->completed_mutex),
                                                                    NULL ),
                              "cpfphig_thread_cond_wait failed",
                              __FILE__,
                              __FUNCTION__,
                              __LINE__ );

                cpfphig_assert( CPFPHIG_OK == cpfphig_mutex_unlock( &(publisher->completed_mutex),
                                                                NULL ),
                              "cpfphig_mutex_unlock failed",
                              __FILE__,
                              __FUNCTION__,
                              __LINE__ );

                // Publisher is in state where it can receive a complete signal from another subscriber
                cpfphig_assert( CPFPHIG_OK == cpfphig_mutex_unlock( &(publisher->completed_ack_mutex),
                                                                NULL ),
                              "cpfphig_mutex_unlock failed",
                              __FILE__,
                              __FUNCTION__,
                              __LINE__ );
            }
        } // thread cond wait triggered
    } // while abort

    // When publisher is null we have no way to signal an error, abort process
    publisher = subscription->publisher;
    cpfphig_assert( publisher != NULL, "subscription->publisher is NULL", __FILE__, __FUNCTION__, __LINE__ );


    // mutex is expected to be locked
    // If somehow the system fails to unlock the mutex, the whole system might deadlock, have to kill process
    cpfphig_assert( CPFPHIG_OK == cpfphig_mutex_unlock( &(publisher->broadcast_mutex),
                                                    NULL ),
                  "cpfphig_mutex_unlock failed",
                  __FILE__,
                  __FUNCTION__,
                  __LINE__ );

    cpfphig_assert( CPFPHIG_OK == cpfphig_free( &error,
                                            NULL ),
                  "cpfphig_free failed",
                  __FILE__,
                  __FUNCTION__,
                  __LINE__ );

    return 0;
}

cpfphig
cpfphig_publisher_subscribe( struct cpfphig_publisher*                      Publisher,
                             void*                                          Data,
                             cpfphig_subscription_routine_symbol*           Subscription_Routine,
                             struct cpfphig_subscription**                  Subscription,
                             CPFPHIG_OPTIONAL struct cpfphig_error*         Error )
{
    cpfphig                                           ret                         = CPFPHIG_FAIL;
    struct cpfphig_thread_attr                        thread_attr                 = CPFPHIG_CONST_CPFPHIG_THREAD_ATTR;
    struct cpfphig_thread_cond_attr                   thread_cond_attr            = CPFPHIG_CONST_CPFPHIG_THREAD_COND_ATTR;
    struct cpfphig_subscription*                      subscription                = NULL;
    static const struct cpfphig_subscription          const_subscription          = CPFPHIG_CONST_CPFPHIG_SUBSCRIPTION;
    struct cpfphig_error                              unlock_error                = CPFPHIG_CONST_CPFPHIG_ERROR;
    cpfphig                                           timed_wait_ret              = CPFPHIG_FAIL;
    struct cpfphig_error                              timed_wait_error            = CPFPHIG_CONST_CPFPHIG_ERROR;


    // NULL checks
    if( Publisher == NULL || Subscription_Routine == NULL || Subscription == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Publisher, Subscription_Routine or Subscription is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }
    ret = cpfphig_malloc( sizeof( struct cpfphig_subscription ),
                        &subscription,
                        Error );

    if( ret == CPFPHIG_OK )
    {
        // Assign
        *subscription                                   = const_subscription;
        subscription->publisher                         = Publisher;
        subscription->data                              = Data;
        subscription->subscription_routine              = Subscription_Routine;

        ret = cpfphig_mutex_lock( &(Publisher->mutex),
                                Error );

        if( ret == CPFPHIG_OK )
        {
            ret = cpfphig_list_push( Publisher->subscriptions,
                                   subscription,
                                   Error );

            if( ret == CPFPHIG_OK )
            {
                ret = cpfphig_thread_cond_init( &subscription->ready_thread_cond,
                                              &thread_cond_attr,
                                              Error );
                if( ret == CPFPHIG_OK )
                {
                    ret = cpfphig_thread_create( &(subscription->thread),
                                               &thread_attr,
                                               &cpfphig_publisher_subscription_routine,
                                               subscription,
                                               Error );

                    if( CPFPHIG_OK == ( ret = cpfphig_mutex_lock( &Publisher->broadcast_mutex,
                                                                Error ) ) )
                    {
                        // Assume fail
                        timed_wait_ret = CPFPHIG_FAIL;

                        // Poke the thread until its ready
                        while( ret == CPFPHIG_OK &&
                               timed_wait_ret == CPFPHIG_FAIL )
                        {
                            // When a non-intended received gets the signal for ready check, just have it ignore it
                            Publisher->published_thread_cond_kind = cpfphig_publisher_thread_cond_kind_ready;

                            // Signal alot , dont wait for long
                            if( CPFPHIG_OK == ( ret = cpfphig_thread_cond_signal( &Publisher->publish_thread_cond,
                                                                                Error ) ) )
                            {
                                // TODO max number of attemps ? otherwise this might deadlock when something goes wrong
                                if( CPFPHIG_FAIL == ( timed_wait_ret = cpfphig_thread_cond_timed_wait( &subscription->ready_thread_cond,
                                                                                                     &Publisher->broadcast_mutex,
                                                                                                     1,
                                                                                                     &timed_wait_error ) ) )
                                {
                                    if( timed_wait_error.error_type == cpfphig_system_error )
                                    {
                                        if( Error != NULL )
                                            *Error = timed_wait_error;

                                        ret = CPFPHIG_FAIL;
                                    }
                                }
                            } // signaled ready poke
                        }

                        if( ret == CPFPHIG_OK )
                        {
                            *Subscription = subscription;
                        } // thread created

                        if( CPFPHIG_FAIL == cpfphig_mutex_unlock( &Publisher->broadcast_mutex,
                                                                &unlock_error ) )
                        {
                            if( ret == CPFPHIG_OK )
                            {
                                if( Error != NULL )
                                    *Error = unlock_error;

                                ret = CPFPHIG_FAIL;
                            }
                        }

                        // TODO ret fail kill thread
                    } // broadcast_mutex locked
                    if( ret == CPFPHIG_FAIL )
                    {
                        cpfphig_thread_cond_destroy( &subscription->ready_thread_cond,
                                                   NULL );
                    }
                } // ready_thread_cond init
            } // driver pushed

            if( CPFPHIG_FAIL == cpfphig_mutex_unlock( &(Publisher->mutex),
                                                    Error ) )
            {
                ret = CPFPHIG_FAIL;
            }
        } // mutex locked
    } // subscription allocated

    if( ret == CPFPHIG_FAIL )
    {
        cpfphig_free( &subscription, NULL );
    }

    return ret;
}


