#include "fphig/fphig.h"
#include "fphig/publisher_create.h"
#include "fphig/publisher.h"
#include "fphig/list.h"
#include "fphig/thread_cond_init.h"
#include "fphig/thread_cond_destroy.h"
#include "fphig/thread_cond_attr.h"
#include "fphig/mutex_init.h"
#include "fphig/mutex_attr.h"
#include "fphig/mutex_destroy.h"
#include "fphig/malloc.h"
#include "fphig/assert.h"

fphig
fphig_publisher_create( struct fphig_publisher*                  Publisher,
                        FPHIG_OPTIONAL struct fphig_error*     Error )
{
    fphig                           ret                 = FPHIG_FAIL;
    struct fphig_thread_cond_attr   thread_cond_attr    = FPHIG_CONST_FPHIG_THREAD_COND_ATTR;
    struct fphig_mutex_attr         mutex_attr          = FPHIG_CONST_FPHIG_MUTEX_ATTR;
    static const struct fphig_list  const_list          = FPHIG_CONST_FPHIG_LIST;

    // NULL checks
    if( Publisher == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "Publisher is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    if( FPHIG_OK == ( ret = fphig_thread_cond_init( &(Publisher->publish_thread_cond),
                                                      &thread_cond_attr,
                                                      Error ) ) )
    {
        if( FPHIG_OK == ( ret = fphig_thread_cond_init( &(Publisher->completed_thread_cond),
                                                          &thread_cond_attr, // yes use the same
                                                          Error ) ) )
        {
            if( FPHIG_OK == ( ret = fphig_thread_cond_init( &(Publisher->completed_ack_thread_cond),
                                                              &thread_cond_attr, // yes use the same
                                                              Error ) ) )
            {
                if( FPHIG_OK == ( ret = fphig_mutex_init( &(Publisher->mutex),
                                                            &mutex_attr,
                                                            Error ) ) )
                {
                    if( FPHIG_OK == ( ret = fphig_mutex_init( &(Publisher->broadcast_mutex),
                                                                &mutex_attr,
                                                                Error ) ) )
                    {

                        if( FPHIG_OK == ( ret = fphig_mutex_init( &(Publisher->completed_mutex),
                                                                &mutex_attr,
                                                                Error ) ) )
                        {
                            if( FPHIG_OK == ( ret = fphig_mutex_init( &(Publisher->completed_ack_mutex),
                                                                        &mutex_attr,
                                                                        Error ) ) )
                            {
                                if( FPHIG_OK == ( ret = fphig_malloc( sizeof( struct fphig_list ),
                                                                        &(Publisher->subscriptions),
                                                                        Error ) ) )
                                {
                                    *(Publisher->subscriptions) = const_list;

                                }

                                if( ret == FPHIG_FAIL )
                                {
                                    fphig_assert( FPHIG_OK == fphig_mutex_destroy( &(Publisher->completed_ack_mutex),
                                                                                     NULL ),
                                                  "fphig_mutex_destroy failed",
                                                  __FILE__,
                                                  __FUNCTION__,
                                                  __LINE__ );
                                }
                            } // completed_ack_mutex inited

                            if( ret == FPHIG_FAIL )
                            {
                                fphig_assert( FPHIG_OK == fphig_mutex_destroy( &(Publisher->completed_mutex),
                                                                                 NULL ),
                                              "fphig_mutex_destroy failed",
                                              __FILE__,
                                              __FUNCTION__,
                                              __LINE__ );
                            }
                        } // completed_mutex inited

                        if( ret == FPHIG_FAIL )
                        {
                            fphig_assert( FPHIG_OK == fphig_mutex_destroy( &(Publisher->broadcast_mutex),
                                                                             NULL ),
                                          "fphig_mutex_destroy failed",
                                          __FILE__,
                                          __FUNCTION__,
                                          __LINE__ );
                        }
                    } // broadcast_mutex inited

                    if( ret == FPHIG_FAIL )
                    {
                        fphig_assert( FPHIG_OK == fphig_mutex_destroy( &(Publisher->mutex),
                                                                         NULL ),
                                      "fphig_mutex_destroy failed",
                                      __FILE__,
                                      __FUNCTION__,
                                      __LINE__ );
                    }
                } // mutex inited

                if( ret == FPHIG_FAIL )
                {
                    fphig_assert( FPHIG_OK == fphig_thread_cond_destroy( &(Publisher->completed_ack_thread_cond),
                                                                           NULL ),
                                  "fphig_thread_cond_destroy failed",
                                  __FILE__,
                                  __FUNCTION__,
                                  __LINE__ );
                }
            } // thread_cond completed_ack_thread_cond inited

            if( ret == FPHIG_FAIL )
            {
                fphig_assert( FPHIG_OK == fphig_thread_cond_destroy( &(Publisher->completed_thread_cond),
                                                                       NULL ),
                              "fphig_thread_cond_destroy failed",
                              __FILE__,
                              __FUNCTION__,
                              __LINE__ );
            }
        } // thread_cond completed_thread_cond inited

        if( ret == FPHIG_FAIL )
        {
            fphig_assert( FPHIG_OK == fphig_thread_cond_destroy( &(Publisher->publish_thread_cond),
                                                                   NULL ),
                          "fphig_thread_cond_destroy failed",
                          __FILE__,
                          __FUNCTION__,
                          __LINE__ );
        }
    } // thread_cond publish_thread_cond inited

    return FPHIG_OK;
}


