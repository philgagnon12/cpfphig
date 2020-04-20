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

#define FPHIG_BUFFER_SIZE ( 0xFF )


fphig
fphig_publisher_publish( struct fphig_publisher*                     Publisher,
                         void*                                       Data,
                         enum fphig_publisher_thread_cond_kind       Publish_Thread_Cond_Kind,
                         struct fphig_error*                         Error )
{
    fphig                                           ret                         = FPHIG_FAIL;
    struct fphig_list_iterator                      subscriptions_iterator      = FPHIG_CONST_MPHIG_LIST_ITERATOR;
    fphig                                           next_subscriptions_ret      = FPHIG_FAIL;
    struct fphig_error                              next_subscriptions_error    = FPHIG_CONST_MPHIG_ERROR;
    struct fphig_output_scheduler_subscription*     subscription                = NULL;
    int                                             subscriptions_count         = 0;
    int                                             completed_count             = 0;

    char    error_message_buffer[FPHIG_BUFFER_SIZE];

    // NULL checks
    if( Publisher == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "Publisher is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    if( FPHIG_FAIL == fphig_mutex_lock( &(Publisher->mutex),
                                          Error ) )
    {
        return FPHIG_FAIL;
    }

    switch( Publish_Thread_Cond_Kind )
    {
        case fphig_publisher_thread_cond_kind_abort:
        case fphig_publisher_thread_cond_kind_data:
            ret = FPHIG_OK;
            Publisher->data                         = Data;
            Publisher->published_thread_cond_kind   = Publish_Thread_Cond_Kind;
            Publisher->completed_thread_cond_kind   = fphig_publisher_thread_cond_kind_abort; // default as fail
            break;
        case fphig_publisher_thread_cond_kind_completed:
        default:
            if( Error != NULL )
            {
                    memset( error_message_buffer,
                            0x00,
                            FPHIG_BUFFER_SIZE );

                    snprintf( error_message_buffer,
                              FPHIG_BUFFER_SIZE,
                              "Unsupported Publish_Thread_Cond_Kind %02X",
                              Publish_Thread_Cond_Kind );

                    fphig_error_message(fphig_system_error, error_message_buffer, Error, __FILE__, __FUNCTION__, __LINE__ );
            }

            ret = FPHIG_FAIL;
    } // switch

    if( ret == FPHIG_OK )
    {
        subscriptions_iterator.list         = Publisher->subscriptions;
        subscriptions_iterator.current_node = NULL;

        // Count the subscriptions
        while( FPHIG_OK == ( next_subscriptions_ret = fphig_list_next( &subscriptions_iterator,
                                                                         &subscription,
                                                                         &next_subscriptions_error ) ) )
        {
            subscriptions_count++;
        }
        if( next_subscriptions_ret == FPHIG_FAIL &&
            next_subscriptions_error.error_type == fphig_system_error )
        {
            if( Error != NULL )
                *Error = next_subscriptions_error;

            ret = FPHIG_FAIL;
        }
        if( ret == FPHIG_OK )
        {
            if( FPHIG_OK == ( ret = fphig_mutex_lock( &(Publisher->completed_mutex),
                                                        Error ) ) )
            {
                if( FPHIG_OK == ( ret = fphig_thread_cond_broadcast( &(Publisher->publish_thread_cond),
                                                                       Error ) ) )
                {
                    while( completed_count < subscriptions_count &&
                           ret == FPHIG_OK )
                    {
                        // wait unlocks the mutex and blocks
                        if( FPHIG_FAIL == fphig_thread_cond_wait( &(Publisher->completed_thread_cond),
                                                                    &(Publisher->completed_mutex),
                                                                    Error ) )
                        {
                            ret = FPHIG_FAIL;
                            break;
                        }

                        // At this point cond_wait has unblocked and mutex is locked
                        switch( Publisher->completed_thread_cond_kind )
                        {
                            case fphig_publisher_thread_cond_kind_completed:
                                completed_count++;

                                ret = fphig_thread_cond_signal( &(Publisher->completed_ack_thread_cond),
                                                                Error );
                                break;
                            case fphig_publisher_thread_cond_kind_abort:
                                if( Error != NULL )
                                    *Error = Publisher->published_error;
                                ret = FPHIG_FAIL;
                                break;
                            default:
                                if( Error != NULL )
                                {
                                        memset( error_message_buffer,
                                                0x00,
                                                FPHIG_BUFFER_SIZE );

                                        snprintf( error_message_buffer,
                                                  FPHIG_BUFFER_SIZE,
                                                  "Unsupported Publisher->completed_thread_cond_kind %02X",
                                                  Publisher->completed_thread_cond_kind );

                                        fphig_error_message(fphig_system_error, error_message_buffer, Error, __FILE__, __FUNCTION__, __LINE__ );
                                }

                                ret = FPHIG_FAIL;
                        } // switch
                    } // while
                } // broadcasted

                // Clean up
                if( FPHIG_FAIL == fphig_mutex_unlock( &(Publisher->completed_mutex),
                                                              Error ) ) // Overwrite error
                {
                    ret = FPHIG_FAIL;
                }

                if( ret == FPHIG_OK )
                {
                    // Synchronize broadcast_mutex before unlocking publisher-mutex,
                    // ensurring that the last subscriber is waiting to receive broadcast again
                    if( FPHIG_OK == ( ret = fphig_mutex_lock( &(Publisher->broadcast_mutex),
                                                            Error ) ) )
                    {
                        ret = fphig_mutex_unlock( &(Publisher->broadcast_mutex),
                                                  Error );
                    }
                }
            } // &(Scheduler->completed_mutex) locked
        } // list traversed
    } // Schedule_Thread_Cond_Kind is good

    if( FPHIG_FAIL == fphig_mutex_unlock( &(Publisher->mutex),
                                            Error ) ) // Overwrite error
    {
        ret = FPHIG_FAIL;
    }

    return ret;
}


