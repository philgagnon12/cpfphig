#include "cpfphig/cpfphig.h"
#include "cpfphig/publisher_publish.h"
#include "cpfphig/publisher.h"
#include "cpfphig/list.h"
#include "cpfphig/thread_cond_broadcast.h"
#include "cpfphig/thread_cond_wait.h"
#include "cpfphig/mutex_lock.h"
#include "cpfphig/mutex_unlock.h"
#include "cpfphig/thread_cond_signal.h"
#include "cpfphig/assert.h"

#include <string.h>
#include <stdio.h>

cpfphig
cpfphig_publisher_publish( struct cpfphig_publisher*                Publisher,
                           void*                                    Data,
                           enum cpfphig_publisher_thread_cond_kind  Publish_Thread_Cond_Kind,
                           struct cpfphig_error*                    Error )
{
    cpfphig                                         ret                         = CPFPHIG_FAIL;
    struct cpfphig_list_iterator                    subscriptions_iterator      = CPFPHIG_CONST_CPFPHIG_LIST_ITERATOR;
    cpfphig                                         next_subscriptions_ret      = CPFPHIG_FAIL;
    struct cpfphig_error                            next_subscriptions_error    = CPFPHIG_CONST_CPFPHIG_ERROR;
    struct cpfphig_output_scheduler_subscription*   subscription                = NULL;
    int                                             subscriptions_count         = 0;
    int                                             completed_count             = 0;
    int                                             error_count                 = 0;
    struct cpfphig_error                            completed_error             = CPFPHIG_CONST_CPFPHIG_ERROR;

    // NULL checks
    if( Publisher == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Publisher is NULL", Error );

        return CPFPHIG_FAIL;
    }

    if( CPFPHIG_FAIL == cpfphig_mutex_lock( &(Publisher->mutex),
                                            Error ) )
    {
        return CPFPHIG_FAIL;
    }

    switch( Publish_Thread_Cond_Kind )
    {
        case cpfphig_publisher_thread_cond_kind_abort:
        case cpfphig_publisher_thread_cond_kind_data:
            ret = CPFPHIG_OK;
            Publisher->data                         = Data;
            Publisher->published_thread_cond_kind   = Publish_Thread_Cond_Kind;
            Publisher->completed_thread_cond_kind   = cpfphig_publisher_thread_cond_kind_abort; // default as fail
            break;
        case cpfphig_publisher_thread_cond_kind_completed:
        default:
            if( Error != NULL )
            {
                cpfphig_error_message( cpfphig_system_error,
                                       "Unsupported Publish_Thread_Cond_Kind %#02hhx",
                                       Error,
                                       (const char)Publish_Thread_Cond_Kind );
            }

            ret = CPFPHIG_FAIL;
    } // switch

    if( ret == CPFPHIG_OK )
    {
        // Reset iterator
        subscriptions_iterator.list         = Publisher->subscriptions;
        subscriptions_iterator.current_node = NULL;

        next_subscriptions_ret              = CPFPHIG_OK;
        next_subscriptions_error.error_type = cpfphig_ok;

        // Count the subscriptions
        while( CPFPHIG_OK == ( next_subscriptions_ret = cpfphig_list_next( &subscriptions_iterator,
                                                                           &subscription,
                                                                           &next_subscriptions_error ) ) )
        {
            subscriptions_count++;
        }
        if( next_subscriptions_error.error_type == cpfphig_system_error &&
            next_subscriptions_ret              == CPFPHIG_FAIL )
        {
            if( Error != NULL )
                *Error = next_subscriptions_error;

            ret = CPFPHIG_FAIL;
        }
        if( ret == CPFPHIG_OK )
        {
            if( CPFPHIG_OK == ( ret = cpfphig_mutex_lock( &(Publisher->completed_mutex),
                                                          Error ) ) )
            {
                if( CPFPHIG_OK == ( ret = cpfphig_thread_cond_broadcast( &(Publisher->publish_thread_cond),
                                                                         Error ) ) )
                {
                    while( completed_count < subscriptions_count &&
                           ret == CPFPHIG_OK )
                    {
                        // wait unlocks the mutex and blocks
                        if( CPFPHIG_FAIL == cpfphig_thread_cond_wait( &(Publisher->completed_thread_cond),
                                                                      &(Publisher->completed_mutex),
                                                                      Error ) )
                        {
                            ret = CPFPHIG_FAIL;
                            break;
                        }

                        // At this point cond_wait has unblocked and mutex is locked
                        switch( Publisher->completed_thread_cond_kind )
                        {
                            case cpfphig_publisher_thread_cond_kind_completed:
                                completed_count++;

                                ret = cpfphig_thread_cond_signal( &(Publisher->completed_ack_thread_cond),
                                                                  Error );
                                break;
                            case cpfphig_publisher_thread_cond_kind_abort:
                                completed_count++;
                                error_count++;

                                // Overwritte with latest received error
                                completed_error = Publisher->published_error;

                                ret = cpfphig_thread_cond_signal( &(Publisher->completed_ack_thread_cond),
                                                                  Error );
                                break;
                            default:
                                if( Error != NULL )
                                {
                                    cpfphig_error_message( cpfphig_system_error,
                                                           "Unsupported Publisher->completed_thread_cond_kind %#02hhX",
                                                           Error,
                                                           (const char)Publisher->completed_thread_cond_kind );
                                }

                                ret = CPFPHIG_FAIL;
                        } // switch
                    } // while
                } // broadcasted

                // Clean up
                if( CPFPHIG_FAIL == cpfphig_mutex_unlock( &(Publisher->completed_mutex),
                                                          Error ) ) // Overwrite error
                {
                    ret = CPFPHIG_FAIL;
                }

                if( ret == CPFPHIG_OK )
                {
                    // Synchronize broadcast_mutex before unlocking publisher-mutex,
                    // ensurring that the last subscriber is waiting to receive broadcast again
                    if( CPFPHIG_OK == ( ret = cpfphig_mutex_lock( &(Publisher->broadcast_mutex),
                                                                  Error ) ) )
                    {
                        ret = cpfphig_mutex_unlock( &(Publisher->broadcast_mutex),
                                                    Error );
                    }
                }
            } // &(Scheduler->completed_mutex) locked
        } // list traversed
    } // Schedule_Thread_Cond_Kind is good

    if( CPFPHIG_FAIL == cpfphig_mutex_unlock( &(Publisher->mutex),
                                              Error ) ) // Overwrite error
    {
        ret = CPFPHIG_FAIL;
    }

    if( error_count > 0 )
    {
        if( Error != NULL )
            *Error = completed_error;

        ret = CPFPHIG_FAIL;
    }
    
    return ret;
}


