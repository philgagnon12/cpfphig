#include "melphig/melphig.h"
#include "melphig/publisher_publish.h"
#include "melphig/publisher.h"
#include "melphig/list.h"
#include "melphig/thread_cond_broadcast.h"
#include "melphig/thread_cond_wait.h"
#include "melphig/mutex_lock.h"
#include "melphig/mutex_unlock.h"
#include "melphig/thread_cond_signal.h"
#include "melphig/assert.h"

#include <string.h>
#include <stdio.h>

#define MELPHIG_BUFFER_SIZE ( 0xFF )


mphig
mphig_publisher_publish( struct mphig_publisher*                     Publisher,
                         void*                                       Data,
                         enum mphig_publisher_thread_cond_kind       Publish_Thread_Cond_Kind,
                         struct mphig_error*                         Error )
{
    mphig                                           ret                         = MELPHIG_FAIL;
    struct mphig_list_iterator                      subscriptions_iterator      = MELPHIG_CONST_MPHIG_LIST_ITERATOR;
    mphig                                           next_subscriptions_ret      = MELPHIG_FAIL;
    struct mphig_error                              next_subscriptions_error    = MELPHIG_CONST_MPHIG_ERROR;
    struct mphig_output_scheduler_subscription*     subscription                = NULL;
    int                                             subscriptions_count         = 0;
    int                                             completed_count             = 0;

    char    error_message_buffer[MELPHIG_BUFFER_SIZE];

    // NULL checks
    if( Publisher == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Publisher is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    if( MELPHIG_FAIL == mphig_mutex_lock( &(Publisher->mutex),
                                          Error ) )
    {
        return MELPHIG_FAIL;
    }

    switch( Publish_Thread_Cond_Kind )
    {
        case mphig_publisher_thread_cond_kind_abort:
        case mphig_publisher_thread_cond_kind_data:
            ret = MELPHIG_OK;
            Publisher->data                         = Data;
            Publisher->published_thread_cond_kind   = Publish_Thread_Cond_Kind;
            Publisher->completed_thread_cond_kind   = mphig_publisher_thread_cond_kind_abort; // default as fail
            break;
        case mphig_publisher_thread_cond_kind_completed:
        default:
            if( Error != NULL )
            {
                    memset( error_message_buffer,
                            0x00,
                            MELPHIG_BUFFER_SIZE );

                    snprintf( error_message_buffer,
                              MELPHIG_BUFFER_SIZE,
                              "Unsupported Publish_Thread_Cond_Kind %02X",
                              Publish_Thread_Cond_Kind );

                    mphig_error_message(mphig_system_error, error_message_buffer, Error, __FILE__, __FUNCTION__, __LINE__ );
            }

            ret = MELPHIG_FAIL;
    } // switch

    if( ret == MELPHIG_OK )
    {
        subscriptions_iterator.list         = Publisher->subscriptions;
        subscriptions_iterator.current_node = NULL;

        // Count the subscriptions
        while( MELPHIG_OK == ( next_subscriptions_ret = mphig_list_next( &subscriptions_iterator,
                                                                         &subscription,
                                                                         &next_subscriptions_error ) ) )
        {
            subscriptions_count++;
        }
        if( next_subscriptions_ret == MELPHIG_FAIL &&
            next_subscriptions_error.error_type == mphig_system_error )
        {
            if( Error != NULL )
                *Error = next_subscriptions_error;

            ret = MELPHIG_FAIL;
        }
        if( ret == MELPHIG_OK )
        {
            if( MELPHIG_OK == ( ret = mphig_mutex_lock( &(Publisher->completed_mutex),
                                                        Error ) ) )
            {
                if( MELPHIG_OK == ( ret = mphig_thread_cond_broadcast( &(Publisher->publish_thread_cond),
                                                                       Error ) ) )
                {
                    while( completed_count < subscriptions_count &&
                           ret == MELPHIG_OK )
                    {
                        // wait unlocks the mutex and blocks
                        if( MELPHIG_FAIL == mphig_thread_cond_wait( &(Publisher->completed_thread_cond),
                                                                    &(Publisher->completed_mutex),
                                                                    Error ) )
                        {
                            ret = MELPHIG_FAIL;
                            break;
                        }

                        // At this point cond_wait has unblocked and mutex is locked
                        switch( Publisher->completed_thread_cond_kind )
                        {
                            case mphig_publisher_thread_cond_kind_completed:
                                completed_count++;

                                ret = mphig_thread_cond_signal( &(Publisher->completed_ack_thread_cond),
                                                                Error );
                                break;
                            case mphig_publisher_thread_cond_kind_abort:
                                if( Error != NULL )
                                    *Error = Publisher->published_error;
                                ret = MELPHIG_FAIL;
                                break;
                            default:
                                if( Error != NULL )
                                {
                                        memset( error_message_buffer,
                                                0x00,
                                                MELPHIG_BUFFER_SIZE );

                                        snprintf( error_message_buffer,
                                                  MELPHIG_BUFFER_SIZE,
                                                  "Unsupported Publisher->completed_thread_cond_kind %02X",
                                                  Publisher->completed_thread_cond_kind );

                                        mphig_error_message(mphig_system_error, error_message_buffer, Error, __FILE__, __FUNCTION__, __LINE__ );
                                }

                                ret = MELPHIG_FAIL;
                        } // switch
                    } // while
                } // broadcasted

                // Clean up
                if( MELPHIG_FAIL == mphig_mutex_unlock( &(Publisher->completed_mutex),
                                                              Error ) ) // Overwrite error
                {
                    ret = MELPHIG_FAIL;
                }

                if( ret == MELPHIG_OK )
                {
                    // Synchronize broadcast_mutex before unlocking publisher-mutex,
                    // ensurring that the last subscriber is waiting to receive broadcast again
                    if( MELPHIG_OK == ( ret = mphig_mutex_lock( &(Publisher->broadcast_mutex),
                                                            Error ) ) )
                    {
                        ret = mphig_mutex_unlock( &(Publisher->broadcast_mutex),
                                                  Error );
                    }
                }
            } // &(Scheduler->completed_mutex) locked
        } // list traversed
    } // Schedule_Thread_Cond_Kind is good

    if( MELPHIG_FAIL == mphig_mutex_unlock( &(Publisher->mutex),
                                            Error ) ) // Overwrite error
    {
        ret = MELPHIG_FAIL;
    }

    return ret;
}


