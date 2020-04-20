#include "melphig/melphig.h"
#include "melphig/publisher_create.h"
#include "melphig/publisher.h"
#include "melphig/list.h"
#include "melphig/thread_cond_init.h"
#include "melphig/thread_cond_destroy.h"
#include "melphig/thread_cond_attr.h"
#include "melphig/mutex_init.h"
#include "melphig/mutex_attr.h"
#include "melphig/mutex_destroy.h"
#include "melphig/malloc.h"
#include "melphig/assert.h"

mphig
mphig_publisher_create( struct mphig_publisher*                  Publisher,
                        MELPHIG_OPTIONAL struct mphig_error*     Error )
{
    mphig                           ret                 = MELPHIG_FAIL;
    struct mphig_thread_cond_attr   thread_cond_attr    = MELPHIG_CONST_MPHIG_THREAD_COND_ATTR;
    struct mphig_mutex_attr         mutex_attr          = MELPHIG_CONST_MPHIG_MUTEX_ATTR;
    static const struct mphig_list  const_list          = MELPHIG_CONST_MPHIG_LIST;

    // NULL checks
    if( Publisher == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Publisher is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    if( MELPHIG_OK == ( ret = mphig_thread_cond_init( &(Publisher->publish_thread_cond),
                                                      &thread_cond_attr,
                                                      Error ) ) )
    {
        if( MELPHIG_OK == ( ret = mphig_thread_cond_init( &(Publisher->completed_thread_cond),
                                                          &thread_cond_attr, // yes use the same
                                                          Error ) ) )
        {
            if( MELPHIG_OK == ( ret = mphig_thread_cond_init( &(Publisher->completed_ack_thread_cond),
                                                              &thread_cond_attr, // yes use the same
                                                              Error ) ) )
            {
                if( MELPHIG_OK == ( ret = mphig_mutex_init( &(Publisher->mutex),
                                                            &mutex_attr,
                                                            Error ) ) )
                {
                    if( MELPHIG_OK == ( ret = mphig_mutex_init( &(Publisher->broadcast_mutex),
                                                                &mutex_attr,
                                                                Error ) ) )
                    {

                        if( MELPHIG_OK == ( ret = mphig_mutex_init( &(Publisher->completed_mutex),
                                                                &mutex_attr,
                                                                Error ) ) )
                        {
                            if( MELPHIG_OK == ( ret = mphig_mutex_init( &(Publisher->completed_ack_mutex),
                                                                        &mutex_attr,
                                                                        Error ) ) )
                            {
                                if( MELPHIG_OK == ( ret = mphig_malloc( sizeof( struct mphig_list ),
                                                                        &(Publisher->subscriptions),
                                                                        Error ) ) )
                                {
                                    *(Publisher->subscriptions) = const_list;

                                }

                                if( ret == MELPHIG_FAIL )
                                {
                                    mphig_assert( MELPHIG_OK == mphig_mutex_destroy( &(Publisher->completed_ack_mutex),
                                                                                     NULL ),
                                                  "mphig_mutex_destroy failed",
                                                  __FILE__,
                                                  __FUNCTION__,
                                                  __LINE__ );
                                }
                            } // completed_ack_mutex inited

                            if( ret == MELPHIG_FAIL )
                            {
                                mphig_assert( MELPHIG_OK == mphig_mutex_destroy( &(Publisher->completed_mutex),
                                                                                 NULL ),
                                              "mphig_mutex_destroy failed",
                                              __FILE__,
                                              __FUNCTION__,
                                              __LINE__ );
                            }
                        } // completed_mutex inited

                        if( ret == MELPHIG_FAIL )
                        {
                            mphig_assert( MELPHIG_OK == mphig_mutex_destroy( &(Publisher->broadcast_mutex),
                                                                             NULL ),
                                          "mphig_mutex_destroy failed",
                                          __FILE__,
                                          __FUNCTION__,
                                          __LINE__ );
                        }
                    } // broadcast_mutex inited

                    if( ret == MELPHIG_FAIL )
                    {
                        mphig_assert( MELPHIG_OK == mphig_mutex_destroy( &(Publisher->mutex),
                                                                         NULL ),
                                      "mphig_mutex_destroy failed",
                                      __FILE__,
                                      __FUNCTION__,
                                      __LINE__ );
                    }
                } // mutex inited

                if( ret == MELPHIG_FAIL )
                {
                    mphig_assert( MELPHIG_OK == mphig_thread_cond_destroy( &(Publisher->completed_ack_thread_cond),
                                                                           NULL ),
                                  "mphig_thread_cond_destroy failed",
                                  __FILE__,
                                  __FUNCTION__,
                                  __LINE__ );
                }
            } // thread_cond completed_ack_thread_cond inited

            if( ret == MELPHIG_FAIL )
            {
                mphig_assert( MELPHIG_OK == mphig_thread_cond_destroy( &(Publisher->completed_thread_cond),
                                                                       NULL ),
                              "mphig_thread_cond_destroy failed",
                              __FILE__,
                              __FUNCTION__,
                              __LINE__ );
            }
        } // thread_cond completed_thread_cond inited

        if( ret == MELPHIG_FAIL )
        {
            mphig_assert( MELPHIG_OK == mphig_thread_cond_destroy( &(Publisher->publish_thread_cond),
                                                                   NULL ),
                          "mphig_thread_cond_destroy failed",
                          __FILE__,
                          __FUNCTION__,
                          __LINE__ );
        }
    } // thread_cond publish_thread_cond inited

    return MELPHIG_OK;
}


