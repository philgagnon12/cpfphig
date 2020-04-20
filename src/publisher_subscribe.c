#include "melphig/melphig.h"
#include "melphig/publisher_subscribe.h"
#include "melphig/publisher.h"
#include "melphig/subscription.h"
#include "melphig/assert.h"
#include "melphig/thread_attr.h"
#include "melphig/malloc.h"
#include "melphig/mutex_lock.h"
#include "melphig/thread_create.h"
#include "melphig/mutex_unlock.h"
#include "melphig/free.h"
#include "melphig/list.h"
#include "melphig/thread_cond_wait.h"
#include "melphig/thread_cond_signal.h"
#include "melphig/thread_cond_timed_wait.h"
#include "melphig/thread_cond_init.h"
#include "melphig/thread_cond_destroy.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

#define MELPHIG_BUFFER_SIZE ( 0xFF )

static
void*
mphig_publisher_subscription_routine( void* Subscription )
{
    struct mphig_subscription*                      subscription                = NULL;
    struct mphig_publisher*                         publisher                   = NULL;
    mphig                                           ret                         = MELPHIG_FAIL;
    int                                             abort                       = 0;
    struct mphig_error*                             error                       = NULL;
    struct mphig_error                              const_error                 = MELPHIG_CONST_MPHIG_ERROR;
    enum mphig_publisher_thread_cond_kind           published_thread_cond_kind  = mphig_publisher_thread_cond_kind_abort;
    mphig                                           ret_to_signal               = MELPHIG_FAIL;

    char    error_message_buffer[MELPHIG_BUFFER_SIZE];

    // When Subscription is null we have no way to signal an error, abort process
    subscription = (struct mphig_subscription*)Subscription;
    mphig_assert( subscription != NULL, "Subscription is NULL", __FILE__, __FUNCTION__, __LINE__ );


    // When publisher is null we have no way to signal an error, abort process
    publisher = subscription->publisher;
    mphig_assert( publisher != NULL, "subscription->publisher is NULL", __FILE__, __FUNCTION__, __LINE__ );

    mphig_assert( MELPHIG_OK == mphig_malloc( sizeof( struct mphig_error ),
                                              &error,
                                              NULL ),
                  "mphig_malloc failed",
                  __FILE__,
                  __FUNCTION__,
                  __LINE__ );

    *error = const_error;

    mphig_assert( MELPHIG_OK == mphig_mutex_lock( &(publisher->broadcast_mutex),
                                                  NULL ),
                  "mphig_mutex_lock failed",
                  __FILE__,
                  __FUNCTION__,
                  __LINE__ );

    // Assume ok
    ret = MELPHIG_OK;

    while( abort == 0 )
    {
        // unlocked when wait is called
        if( MELPHIG_OK == ( ret = mphig_thread_cond_wait( &(publisher->publish_thread_cond),
                                                          &(publisher->broadcast_mutex),
                                                          error ) ) )
        {

            if( subscription->thread_ready == 0 )
            {
                subscription->thread_ready = 1;
                // TODO is sending the signal once good enough ?
                mphig_assert( MELPHIG_OK == mphig_thread_cond_signal( &subscription->ready_thread_cond,
                                                                      NULL ),
                      "mphig_mutex_unlock failed",
                      __FILE__,
                      __FUNCTION__,
                      __LINE__ );
                continue;
            }
            // When publisher is null we have no way to signal an error, abort process
            publisher = subscription->publisher;
            mphig_assert( publisher != NULL, "subscription->publisher is NULL", __FILE__, __FUNCTION__, __LINE__ );

            switch( publisher->published_thread_cond_kind )
            {
                case mphig_publisher_thread_cond_kind_ready:
                    // Ignore, most likely not intended recipient
                    continue;
                    break;
                case mphig_publisher_thread_cond_kind_abort:

                    // Yes below is safe we have the general mutex

                    // ret is OK
                    ret_to_signal = MELPHIG_OK;

                    if( subscription == publisher->unsubscribing_subscription)
                    {
                        abort = 1;

                        // remove itself from scheduler subscriptions
                        ret_to_signal = mphig_list_remove( publisher->subscriptions,
                                                           subscription,
                                                           error );

                        publisher->unsubscribing_subscription = NULL;
                    }

                    if( ret_to_signal == MELPHIG_OK )
                    {
                        // Report as complete even if the abort was not meant for this subscription
                        publisher->completed_thread_cond_kind = mphig_publisher_thread_cond_kind_completed;
                    }
                    break;
                case mphig_publisher_thread_cond_kind_data:

                    mphig_assert( subscription->subscription_routine != NULL, "subscription->subscription_routine is NULL", __FILE__, __FUNCTION__, __LINE__ );

                    // Unlock broadcast mutex to allow subscription routines to run in parrallel
                    mphig_assert( MELPHIG_OK == mphig_mutex_unlock( &(publisher->broadcast_mutex),
                                                                NULL ),
                              "mphig_mutex_unlock failed",
                              __FILE__,
                              __FUNCTION__,
                              __LINE__ );

                    ret_to_signal = subscription->subscription_routine( publisher->data,
                                                                        subscription->data,
                                                                        error );

                    mphig_assert( MELPHIG_OK == mphig_mutex_lock( &(publisher->broadcast_mutex),
                                                                NULL ),
                              "mphig_mutex_unlock failed",
                              __FILE__,
                              __FUNCTION__,
                              __LINE__ );

                    break;
                case mphig_publisher_thread_cond_kind_completed:
                default:
                    memset( error_message_buffer,
                            0x00,
                            MELPHIG_BUFFER_SIZE );

                    snprintf( error_message_buffer,
                              MELPHIG_BUFFER_SIZE,
                              "Unsupported published_thread_cond_kind %02X",
                              published_thread_cond_kind );

                    mphig_error_message(mphig_system_error, error_message_buffer, error, __FILE__, __FUNCTION__, __LINE__ );

                    ret = MELPHIG_FAIL;
            } // switch

            if( ret == MELPHIG_OK )
            {
                if( ret_to_signal == MELPHIG_FAIL )
                {
                    abort = 1;
                    publisher->published_error            = *error;
                    publisher->completed_thread_cond_kind = mphig_publisher_thread_cond_kind_abort;
                }
                else
                {
                    publisher->completed_thread_cond_kind = mphig_publisher_thread_cond_kind_completed;
                }

                // Only a single subscriber can receive a complete ack from the publisher
                mphig_assert( MELPHIG_OK == mphig_mutex_lock( &(publisher->completed_ack_mutex),
                                                              NULL ),
                              "mphig_mutex_lock failed",
                              __FILE__,
                              __FUNCTION__,
                              __LINE__ );


                // Lock to be able to receive our completed ack signal from publisher
                mphig_assert( MELPHIG_OK == mphig_mutex_lock( &(publisher->completed_mutex),
                                                              NULL ),
                              "mphig_mutex_lock failed",
                              __FILE__,
                              __FUNCTION__,
                              __LINE__ );

                // Send complete signal
                mphig_assert( MELPHIG_OK == mphig_thread_cond_signal( &(publisher->completed_thread_cond),
                                                                      NULL ),
                              "mphig_thread_cond_signal failed",
                              __FILE__,
                              __FUNCTION__,
                              __LINE__ );

                // wait unlocks the mutex and blocks
                mphig_assert( MELPHIG_OK == mphig_thread_cond_wait( &(publisher->completed_ack_thread_cond),
                                                                    &(publisher->completed_mutex),
                                                                    NULL ),
                              "mphig_thread_cond_wait failed",
                              __FILE__,
                              __FUNCTION__,
                              __LINE__ );

                mphig_assert( MELPHIG_OK == mphig_mutex_unlock( &(publisher->completed_mutex),
                                                                NULL ),
                              "mphig_mutex_unlock failed",
                              __FILE__,
                              __FUNCTION__,
                              __LINE__ );

                // Publisher is in state where it can receive a complete signal from another subscriber
                mphig_assert( MELPHIG_OK == mphig_mutex_unlock( &(publisher->completed_ack_mutex),
                                                                NULL ),
                              "mphig_mutex_unlock failed",
                              __FILE__,
                              __FUNCTION__,
                              __LINE__ );
            }
        } // thread cond wait triggered
    } // while abort

    // When publisher is null we have no way to signal an error, abort process
    publisher = subscription->publisher;
    mphig_assert( publisher != NULL, "subscription->publisher is NULL", __FILE__, __FUNCTION__, __LINE__ );


    // mutex is expected to be locked
    // If somehow the system fails to unlock the mutex, the whole system might deadlock, have to kill process
    mphig_assert( MELPHIG_OK == mphig_mutex_unlock( &(publisher->broadcast_mutex),
                                                    NULL ),
                  "mphig_mutex_unlock failed",
                  __FILE__,
                  __FUNCTION__,
                  __LINE__ );

    mphig_assert( MELPHIG_OK == mphig_free( &error,
                                            NULL ),
                  "mphig_free failed",
                  __FILE__,
                  __FUNCTION__,
                  __LINE__ );

    return NULL;
}

mphig
mphig_publisher_subscribe( struct mphig_publisher*                      Publisher,
                           void*                                        Data,
                           mphig_subscription_routine_symbol*           Subscription_Routine,
                           struct mphig_subscription**                  Subscription,
                           MELPHIG_OPTIONAL struct mphig_error*         Error )
{
    mphig                                           ret                         = MELPHIG_FAIL;
    struct mphig_thread_attr                        thread_attr                 = {}; // TODO add const or does it cause problem ?
    struct mphig_thread_cond_attr                   thread_cond_attr            = MELPHIG_CONST_MPHIG_THREAD_COND_ATTR;
    struct mphig_subscription*                      subscription                = NULL;
    static const struct mphig_subscription          const_subscription          = MELPHIG_CONST_MPHIG_SUBSCRIPTION;
    struct mphig_error                              unlock_error                = MELPHIG_CONST_MPHIG_ERROR;
    mphig                                           timed_wait_ret              = MELPHIG_FAIL;
    struct mphig_error                              timed_wait_error            = MELPHIG_CONST_MPHIG_ERROR;


    // NULL checks
    if( Publisher == NULL || Subscription_Routine == NULL || Subscription == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Publisher, Subscription_Routine or Subscription is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }
    ret = mphig_malloc( sizeof( struct mphig_subscription ),
                        &subscription,
                        Error );

    if( ret == MELPHIG_OK )
    {
        // Assign
        *subscription                                   = const_subscription;
        subscription->publisher                         = Publisher;
        subscription->data                              = Data;
        subscription->subscription_routine              = Subscription_Routine;

        ret = mphig_mutex_lock( &(Publisher->mutex),
                                Error );

        if( ret == MELPHIG_OK )
        {
            ret = mphig_list_push( Publisher->subscriptions,
                                   subscription,
                                   Error );

            if( ret == MELPHIG_OK )
            {
                ret = mphig_thread_cond_init( &subscription->ready_thread_cond,
                                              &thread_cond_attr,
                                              Error );
                if( ret == MELPHIG_OK )
                {
                    ret = mphig_thread_create( &(subscription->thread),
                                               &thread_attr,
                                               &mphig_publisher_subscription_routine,
                                               subscription,
                                               Error );

                    if( MELPHIG_OK == ( ret = mphig_mutex_lock( &Publisher->broadcast_mutex,
                                                                Error ) ) )
                    {
                        // Assume fail
                        timed_wait_ret = MELPHIG_FAIL;

                        // Poke the thread until its ready
                        while( ret == MELPHIG_OK &&
                               timed_wait_ret == MELPHIG_FAIL )
                        {
                            // When a non-intended received gets the signal for ready check, just have it ignore it
                            Publisher->published_thread_cond_kind = mphig_publisher_thread_cond_kind_ready;

                            // Signal alot , dont wait for long
                            if( MELPHIG_OK == ( ret = mphig_thread_cond_signal( &Publisher->publish_thread_cond,
                                                                                Error ) ) )
                            {
                                // TODO max number of attemps ? otherwise this might deadlock when something goes wrong
                                if( MELPHIG_FAIL == ( timed_wait_ret = mphig_thread_cond_timed_wait( &subscription->ready_thread_cond,
                                                                                                     &Publisher->broadcast_mutex,
                                                                                                     1,
                                                                                                     &timed_wait_error ) ) )
                                {
                                    if( timed_wait_error.error_type == mphig_system_error )
                                    {
                                        if( Error != NULL )
                                            *Error = timed_wait_error;

                                        ret = MELPHIG_FAIL;
                                    }
                                }
                            } // signaled ready poke
                        }

                        if( ret == MELPHIG_OK )
                        {
                            *Subscription = subscription;
                        } // thread created

                        if( MELPHIG_FAIL == mphig_mutex_unlock( &Publisher->broadcast_mutex,
                                                                &unlock_error ) )
                        {
                            if( ret == MELPHIG_OK )
                            {
                                if( Error != NULL )
                                    *Error = unlock_error;

                                ret = MELPHIG_FAIL;
                            }
                        }

                        // TODO ret fail kill thread
                    } // broadcast_mutex locked
                    if( ret == MELPHIG_FAIL )
                    {
                        mphig_thread_cond_destroy( &subscription->ready_thread_cond,
                                                   NULL );
                    }
                } // ready_thread_cond init
            } // driver pushed

            if( MELPHIG_FAIL == mphig_mutex_unlock( &(Publisher->mutex),
                                                    Error ) )
            {
                ret = MELPHIG_FAIL;
            }
        } // mutex locked
    } // subscription allocated

    if( ret == MELPHIG_FAIL )
    {
        mphig_free( &subscription, NULL );
    }

    return ret;
}


