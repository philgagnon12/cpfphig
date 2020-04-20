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

#define FPHIG_BUFFER_SIZE ( 0xFF )

static
void*
fphig_publisher_subscription_routine( void* Subscription )
{
    struct fphig_subscription*                      subscription                = NULL;
    struct fphig_publisher*                         publisher                   = NULL;
    fphig                                           ret                         = FPHIG_FAIL;
    int                                             abort                       = 0;
    struct fphig_error*                             error                       = NULL;
    struct fphig_error                              const_error                 = FPHIG_CONST_MPHIG_ERROR;
    enum fphig_publisher_thread_cond_kind           published_thread_cond_kind  = fphig_publisher_thread_cond_kind_abort;
    fphig                                           ret_to_signal               = FPHIG_FAIL;

    char    error_message_buffer[FPHIG_BUFFER_SIZE];

    // When Subscription is null we have no way to signal an error, abort process
    subscription = (struct fphig_subscription*)Subscription;
    fphig_assert( subscription != NULL, "Subscription is NULL", __FILE__, __FUNCTION__, __LINE__ );


    // When publisher is null we have no way to signal an error, abort process
    publisher = subscription->publisher;
    fphig_assert( publisher != NULL, "subscription->publisher is NULL", __FILE__, __FUNCTION__, __LINE__ );

    fphig_assert( FPHIG_OK == fphig_malloc( sizeof( struct fphig_error ),
                                              &error,
                                              NULL ),
                  "fphig_malloc failed",
                  __FILE__,
                  __FUNCTION__,
                  __LINE__ );

    *error = const_error;

    fphig_assert( FPHIG_OK == fphig_mutex_lock( &(publisher->broadcast_mutex),
                                                  NULL ),
                  "fphig_mutex_lock failed",
                  __FILE__,
                  __FUNCTION__,
                  __LINE__ );

    // Assume ok
    ret = FPHIG_OK;

    while( abort == 0 )
    {
        // unlocked when wait is called
        if( FPHIG_OK == ( ret = fphig_thread_cond_wait( &(publisher->publish_thread_cond),
                                                          &(publisher->broadcast_mutex),
                                                          error ) ) )
        {

            if( subscription->thread_ready == 0 )
            {
                subscription->thread_ready = 1;
                // TODO is sending the signal once good enough ?
                fphig_assert( FPHIG_OK == fphig_thread_cond_signal( &subscription->ready_thread_cond,
                                                                      NULL ),
                      "fphig_mutex_unlock failed",
                      __FILE__,
                      __FUNCTION__,
                      __LINE__ );
                continue;
            }
            // When publisher is null we have no way to signal an error, abort process
            publisher = subscription->publisher;
            fphig_assert( publisher != NULL, "subscription->publisher is NULL", __FILE__, __FUNCTION__, __LINE__ );

            switch( publisher->published_thread_cond_kind )
            {
                case fphig_publisher_thread_cond_kind_ready:
                    // Ignore, most likely not intended recipient
                    continue;
                    break;
                case fphig_publisher_thread_cond_kind_abort:

                    // Yes below is safe we have the general mutex

                    // ret is OK
                    ret_to_signal = FPHIG_OK;

                    if( subscription == publisher->unsubscribing_subscription)
                    {
                        abort = 1;

                        // remove itself from scheduler subscriptions
                        ret_to_signal = fphig_list_remove( publisher->subscriptions,
                                                           subscription,
                                                           error );

                        publisher->unsubscribing_subscription = NULL;
                    }

                    if( ret_to_signal == FPHIG_OK )
                    {
                        // Report as complete even if the abort was not meant for this subscription
                        publisher->completed_thread_cond_kind = fphig_publisher_thread_cond_kind_completed;
                    }
                    break;
                case fphig_publisher_thread_cond_kind_data:

                    fphig_assert( subscription->subscription_routine != NULL, "subscription->subscription_routine is NULL", __FILE__, __FUNCTION__, __LINE__ );

                    // Unlock broadcast mutex to allow subscription routines to run in parrallel
                    fphig_assert( FPHIG_OK == fphig_mutex_unlock( &(publisher->broadcast_mutex),
                                                                NULL ),
                              "fphig_mutex_unlock failed",
                              __FILE__,
                              __FUNCTION__,
                              __LINE__ );

                    ret_to_signal = subscription->subscription_routine( publisher->data,
                                                                        subscription->data,
                                                                        error );

                    fphig_assert( FPHIG_OK == fphig_mutex_lock( &(publisher->broadcast_mutex),
                                                                NULL ),
                              "fphig_mutex_unlock failed",
                              __FILE__,
                              __FUNCTION__,
                              __LINE__ );

                    break;
                case fphig_publisher_thread_cond_kind_completed:
                default:
                    memset( error_message_buffer,
                            0x00,
                            FPHIG_BUFFER_SIZE );

                    snprintf( error_message_buffer,
                              FPHIG_BUFFER_SIZE,
                              "Unsupported published_thread_cond_kind %02X",
                              published_thread_cond_kind );

                    fphig_error_message(fphig_system_error, error_message_buffer, error, __FILE__, __FUNCTION__, __LINE__ );

                    ret = FPHIG_FAIL;
            } // switch

            if( ret == FPHIG_OK )
            {
                if( ret_to_signal == FPHIG_FAIL )
                {
                    abort = 1;
                    publisher->published_error            = *error;
                    publisher->completed_thread_cond_kind = fphig_publisher_thread_cond_kind_abort;
                }
                else
                {
                    publisher->completed_thread_cond_kind = fphig_publisher_thread_cond_kind_completed;
                }

                // Only a single subscriber can receive a complete ack from the publisher
                fphig_assert( FPHIG_OK == fphig_mutex_lock( &(publisher->completed_ack_mutex),
                                                              NULL ),
                              "fphig_mutex_lock failed",
                              __FILE__,
                              __FUNCTION__,
                              __LINE__ );


                // Lock to be able to receive our completed ack signal from publisher
                fphig_assert( FPHIG_OK == fphig_mutex_lock( &(publisher->completed_mutex),
                                                              NULL ),
                              "fphig_mutex_lock failed",
                              __FILE__,
                              __FUNCTION__,
                              __LINE__ );

                // Send complete signal
                fphig_assert( FPHIG_OK == fphig_thread_cond_signal( &(publisher->completed_thread_cond),
                                                                      NULL ),
                              "fphig_thread_cond_signal failed",
                              __FILE__,
                              __FUNCTION__,
                              __LINE__ );

                // wait unlocks the mutex and blocks
                fphig_assert( FPHIG_OK == fphig_thread_cond_wait( &(publisher->completed_ack_thread_cond),
                                                                    &(publisher->completed_mutex),
                                                                    NULL ),
                              "fphig_thread_cond_wait failed",
                              __FILE__,
                              __FUNCTION__,
                              __LINE__ );

                fphig_assert( FPHIG_OK == fphig_mutex_unlock( &(publisher->completed_mutex),
                                                                NULL ),
                              "fphig_mutex_unlock failed",
                              __FILE__,
                              __FUNCTION__,
                              __LINE__ );

                // Publisher is in state where it can receive a complete signal from another subscriber
                fphig_assert( FPHIG_OK == fphig_mutex_unlock( &(publisher->completed_ack_mutex),
                                                                NULL ),
                              "fphig_mutex_unlock failed",
                              __FILE__,
                              __FUNCTION__,
                              __LINE__ );
            }
        } // thread cond wait triggered
    } // while abort

    // When publisher is null we have no way to signal an error, abort process
    publisher = subscription->publisher;
    fphig_assert( publisher != NULL, "subscription->publisher is NULL", __FILE__, __FUNCTION__, __LINE__ );


    // mutex is expected to be locked
    // If somehow the system fails to unlock the mutex, the whole system might deadlock, have to kill process
    fphig_assert( FPHIG_OK == fphig_mutex_unlock( &(publisher->broadcast_mutex),
                                                    NULL ),
                  "fphig_mutex_unlock failed",
                  __FILE__,
                  __FUNCTION__,
                  __LINE__ );

    fphig_assert( FPHIG_OK == fphig_free( &error,
                                            NULL ),
                  "fphig_free failed",
                  __FILE__,
                  __FUNCTION__,
                  __LINE__ );

    return NULL;
}

fphig
fphig_publisher_subscribe( struct fphig_publisher*                      Publisher,
                           void*                                        Data,
                           fphig_subscription_routine_symbol*           Subscription_Routine,
                           struct fphig_subscription**                  Subscription,
                           FPHIG_OPTIONAL struct fphig_error*         Error )
{
    fphig                                           ret                         = FPHIG_FAIL;
    struct fphig_thread_attr                        thread_attr                 = {}; // TODO add const or does it cause problem ?
    struct fphig_thread_cond_attr                   thread_cond_attr            = FPHIG_CONST_MPHIG_THREAD_COND_ATTR;
    struct fphig_subscription*                      subscription                = NULL;
    static const struct fphig_subscription          const_subscription          = FPHIG_CONST_MPHIG_SUBSCRIPTION;
    struct fphig_error                              unlock_error                = FPHIG_CONST_MPHIG_ERROR;
    fphig                                           timed_wait_ret              = FPHIG_FAIL;
    struct fphig_error                              timed_wait_error            = FPHIG_CONST_MPHIG_ERROR;


    // NULL checks
    if( Publisher == NULL || Subscription_Routine == NULL || Subscription == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "Publisher, Subscription_Routine or Subscription is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }
    ret = fphig_malloc( sizeof( struct fphig_subscription ),
                        &subscription,
                        Error );

    if( ret == FPHIG_OK )
    {
        // Assign
        *subscription                                   = const_subscription;
        subscription->publisher                         = Publisher;
        subscription->data                              = Data;
        subscription->subscription_routine              = Subscription_Routine;

        ret = fphig_mutex_lock( &(Publisher->mutex),
                                Error );

        if( ret == FPHIG_OK )
        {
            ret = fphig_list_push( Publisher->subscriptions,
                                   subscription,
                                   Error );

            if( ret == FPHIG_OK )
            {
                ret = fphig_thread_cond_init( &subscription->ready_thread_cond,
                                              &thread_cond_attr,
                                              Error );
                if( ret == FPHIG_OK )
                {
                    ret = fphig_thread_create( &(subscription->thread),
                                               &thread_attr,
                                               &fphig_publisher_subscription_routine,
                                               subscription,
                                               Error );

                    if( FPHIG_OK == ( ret = fphig_mutex_lock( &Publisher->broadcast_mutex,
                                                                Error ) ) )
                    {
                        // Assume fail
                        timed_wait_ret = FPHIG_FAIL;

                        // Poke the thread until its ready
                        while( ret == FPHIG_OK &&
                               timed_wait_ret == FPHIG_FAIL )
                        {
                            // When a non-intended received gets the signal for ready check, just have it ignore it
                            Publisher->published_thread_cond_kind = fphig_publisher_thread_cond_kind_ready;

                            // Signal alot , dont wait for long
                            if( FPHIG_OK == ( ret = fphig_thread_cond_signal( &Publisher->publish_thread_cond,
                                                                                Error ) ) )
                            {
                                // TODO max number of attemps ? otherwise this might deadlock when something goes wrong
                                if( FPHIG_FAIL == ( timed_wait_ret = fphig_thread_cond_timed_wait( &subscription->ready_thread_cond,
                                                                                                     &Publisher->broadcast_mutex,
                                                                                                     1,
                                                                                                     &timed_wait_error ) ) )
                                {
                                    if( timed_wait_error.error_type == fphig_system_error )
                                    {
                                        if( Error != NULL )
                                            *Error = timed_wait_error;

                                        ret = FPHIG_FAIL;
                                    }
                                }
                            } // signaled ready poke
                        }

                        if( ret == FPHIG_OK )
                        {
                            *Subscription = subscription;
                        } // thread created

                        if( FPHIG_FAIL == fphig_mutex_unlock( &Publisher->broadcast_mutex,
                                                                &unlock_error ) )
                        {
                            if( ret == FPHIG_OK )
                            {
                                if( Error != NULL )
                                    *Error = unlock_error;

                                ret = FPHIG_FAIL;
                            }
                        }

                        // TODO ret fail kill thread
                    } // broadcast_mutex locked
                    if( ret == FPHIG_FAIL )
                    {
                        fphig_thread_cond_destroy( &subscription->ready_thread_cond,
                                                   NULL );
                    }
                } // ready_thread_cond init
            } // driver pushed

            if( FPHIG_FAIL == fphig_mutex_unlock( &(Publisher->mutex),
                                                    Error ) )
            {
                ret = FPHIG_FAIL;
            }
        } // mutex locked
    } // subscription allocated

    if( ret == FPHIG_FAIL )
    {
        fphig_free( &subscription, NULL );
    }

    return ret;
}


