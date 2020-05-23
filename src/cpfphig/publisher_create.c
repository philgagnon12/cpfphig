#include "cpfphig/cpfphig.h"
#include "cpfphig/publisher_create.h"
#include "cpfphig/publisher.h"
#include "cpfphig/list.h"
#include "cpfphig/thread_cond_init.h"
#include "cpfphig/thread_cond_destroy.h"
#include "cpfphig/thread_cond_attr.h"
#include "cpfphig/mutex_init.h"
#include "cpfphig/mutex_attr.h"
#include "cpfphig/mutex_destroy.h"
#include "cpfphig/malloc.h"
#include "cpfphig/assert.h"

cpfphig
cpfphig_publisher_create( struct cpfphig_publisher*                 Publisher,
                          CPFPHIG_OPTIONAL struct cpfphig_error*    Error )
{
    cpfphig                           ret                 = CPFPHIG_FAIL;
    struct cpfphig_thread_cond_attr   thread_cond_attr    = CPFPHIG_CONST_CPFPHIG_THREAD_COND_ATTR;
    struct cpfphig_mutex_attr         mutex_attr          = CPFPHIG_CONST_CPFPHIG_MUTEX_ATTR;
    static const struct cpfphig_list  const_list          = CPFPHIG_CONST_CPFPHIG_LIST;

    // NULL checks
    if( Publisher == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Publisher is NULL", Error );

        return CPFPHIG_FAIL;
    }

    if( CPFPHIG_OK == ( ret = cpfphig_thread_cond_init( &(Publisher->publish_thread_cond),
                                                        &thread_cond_attr,
                                                        Error ) ) )
    {
        if( CPFPHIG_OK == ( ret = cpfphig_thread_cond_init( &(Publisher->completed_thread_cond),
                                                            &thread_cond_attr, // yes use the same
                                                            Error ) ) )
        {
            if( CPFPHIG_OK == ( ret = cpfphig_thread_cond_init( &(Publisher->completed_ack_thread_cond),
                                                                &thread_cond_attr, // yes use the same
                                                                Error ) ) )
            {
                if( CPFPHIG_OK == ( ret = cpfphig_mutex_init( &(Publisher->mutex),
                                                              &mutex_attr,
                                                              Error ) ) )
                {
                    if( CPFPHIG_OK == ( ret = cpfphig_mutex_init( &(Publisher->broadcast_mutex),
                                                                  &mutex_attr,
                                                                  Error ) ) )
                    {

                        if( CPFPHIG_OK == ( ret = cpfphig_mutex_init( &(Publisher->completed_mutex),
                                                                      &mutex_attr,
                                                                      Error ) ) )
                        {
                            if( CPFPHIG_OK == ( ret = cpfphig_mutex_init( &(Publisher->completed_ack_mutex),
                                                                          &mutex_attr,
                                                                          Error ) ) )
                            {
                                if( CPFPHIG_OK == ( ret = cpfphig_malloc( sizeof( struct cpfphig_list ),
                                                                          &(Publisher->subscriptions),
                                                                          Error ) ) )
                                {
                                    *(Publisher->subscriptions) = const_list;

                                }

                                if( ret == CPFPHIG_FAIL )
                                {
                                    cpfphig_assert( CPFPHIG_OK == cpfphig_mutex_destroy( &(Publisher->completed_ack_mutex),
                                                                                         NULL ),
                                                    "cpfphig_mutex_destroy failed",
                                                    __FILE__,
                                                    __FUNCTION__,
                                                    __LINE__ );
                                }
                            } // completed_ack_mutex inited

                            if( ret == CPFPHIG_FAIL )
                            {
                                cpfphig_assert( CPFPHIG_OK == cpfphig_mutex_destroy( &(Publisher->completed_mutex),
                                                                                     NULL ),
                                                "cpfphig_mutex_destroy failed",
                                                __FILE__,
                                                __FUNCTION__,
                                                __LINE__ );
                            }
                        } // completed_mutex inited

                        if( ret == CPFPHIG_FAIL )
                        {
                            cpfphig_assert( CPFPHIG_OK == cpfphig_mutex_destroy( &(Publisher->broadcast_mutex),
                                                                                 NULL ),
                                            "cpfphig_mutex_destroy failed",
                                            __FILE__,
                                            __FUNCTION__,
                                            __LINE__ );
                        }
                    } // broadcast_mutex inited

                    if( ret == CPFPHIG_FAIL )
                    {
                        cpfphig_assert( CPFPHIG_OK == cpfphig_mutex_destroy( &(Publisher->mutex),
                                                                             NULL ),
                                        "cpfphig_mutex_destroy failed",
                                        __FILE__,
                                        __FUNCTION__,
                                        __LINE__ );
                    }
                } // mutex inited

                if( ret == CPFPHIG_FAIL )
                {
                    cpfphig_assert( CPFPHIG_OK == cpfphig_thread_cond_destroy( &(Publisher->completed_ack_thread_cond),
                                                                               NULL ),
                                    "cpfphig_thread_cond_destroy failed",
                                    __FILE__,
                                    __FUNCTION__,
                                    __LINE__ );
                }
            } // thread_cond completed_ack_thread_cond inited

            if( ret == CPFPHIG_FAIL )
            {
                cpfphig_assert( CPFPHIG_OK == cpfphig_thread_cond_destroy( &(Publisher->completed_thread_cond),
                                                                           NULL ),
                                "cpfphig_thread_cond_destroy failed",
                                __FILE__,
                                __FUNCTION__,
                                __LINE__ );
            }
        } // thread_cond completed_thread_cond inited

        if( ret == CPFPHIG_FAIL )
        {
            cpfphig_assert( CPFPHIG_OK == cpfphig_thread_cond_destroy( &(Publisher->publish_thread_cond),
                                                                       NULL ),
                            "cpfphig_thread_cond_destroy failed",
                            __FILE__,
                            __FUNCTION__,
                            __LINE__ );
        }
    } // thread_cond publish_thread_cond inited

    return CPFPHIG_OK;
}


