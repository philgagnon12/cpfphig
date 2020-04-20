#include "melphig/melphig.h"
#include "melphig/thread_pool_thread.h"
#include "melphig/thread_pool_thread_create.h"
#include "melphig/thread_pool.h"
#include "melphig/malloc.h"
#include "melphig/free.h"
#include "melphig/mutex.h"
#include "melphig/mutex_init.h"
#include "melphig/mutex_destroy.h"
#include "melphig/mutex_attr.h"
#include "melphig/mutex_lock.h"
#include "melphig/mutex_unlock.h"
#include "melphig/thread_create.h"
#include "melphig/thread_join.h"
#include "melphig/thread_attr.h"
#include "melphig/thread_cond_init.h"
#include "melphig/thread_cond_attr.h"
#include "melphig/thread_cond_destroy.h"
#include "melphig/thread_cond_signal.h"
#include "melphig/thread_cond_wait.h"
#include "melphig/thread_cond_timed_wait.h"
#include "melphig/assert.h"

static
void*
thread_pool_thread_routine( void* Thread_Pool_Thread )
{
    struct mphig_thread_pool_thread*    thread_pool_thread  = NULL;
    int                                 abort               = 0;

    thread_pool_thread = (struct mphig_thread_pool_thread*)Thread_Pool_Thread;
    mphig_assert( thread_pool_thread != NULL, "Thread_Pool_Thread is NULL", __FILE__, __FUNCTION__, __LINE__ );

    mphig_assert( MELPHIG_OK == mphig_mutex_lock( &thread_pool_thread->mutex,
                                                  NULL ),
                  "mphig_mutex_lock failed",
                  __FILE__,
                  __FUNCTION__,
                  __LINE__ );

    while( abort == 0 )
    {
        mphig_assert( MELPHIG_OK == mphig_thread_cond_wait( &thread_pool_thread->thread_cond,
                                                            &thread_pool_thread->mutex,
                                                            NULL ),
                      "mphig_thread_cond_wait failed",
                      __FILE__,
                      __FUNCTION__,
                      __LINE__ );

        switch( thread_pool_thread->thread_cond_kind )
        {
            case mphig_thread_pool_thread_cond_kind_ready:
                mphig_assert( MELPHIG_OK == mphig_thread_cond_signal( &thread_pool_thread->ready_thread_cond,
                                                                      NULL ),
                  "mphig_thread_cond_signal failed",
                  __FILE__,
                  __FUNCTION__,
                  __LINE__ );
                break;
            case mphig_thread_pool_thread_cond_kind_routine:
                mphig_assert( thread_pool_thread->routine != NULL, "thread_pool_thread->routine is NULL", __FILE__, __FUNCTION__, __LINE__ );

                thread_pool_thread->routine( thread_pool_thread->routine_arg );

                mphig_assert( MELPHIG_OK == mphig_mutex_lock( &thread_pool_thread->busy_mutex,
                                                              NULL ),
                          "mphig_mutex_lock failed",
                          __FILE__,
                          __FUNCTION__,
                          __LINE__ );

                // Not busy anymore
                thread_pool_thread->busy = 0;

                mphig_assert( MELPHIG_OK == mphig_mutex_unlock( &thread_pool_thread->busy_mutex,
                                                                NULL ),
                          "mphig_mutex_unlock failed",
                          __FILE__,
                          __FUNCTION__,
                          __LINE__ );
                break;
            case mphig_thread_pool_thread_cond_kind_abort:
                abort = 1;
                break;
            default:
                mphig_assert( thread_pool_thread->thread_cond_kind == mphig_thread_pool_thread_cond_kind_ready ||
                              thread_pool_thread->thread_cond_kind == mphig_thread_pool_thread_cond_kind_routine ||
                              thread_pool_thread->thread_cond_kind == mphig_thread_pool_thread_cond_kind_abort, 
                              "thread_pool_thread->thread_cond_kind not supported",
                              __FILE__,
                              __FUNCTION__,
                              __LINE__ );
        } // switch
    } // while

    mphig_assert( MELPHIG_OK == mphig_mutex_unlock( &thread_pool_thread->mutex,
                                                    NULL ),
                  "mphig_mutex_unlock failed",
                  __FILE__,
                  __FUNCTION__,
                  __LINE__ );


    return NULL;
}

mphig
mphig_thread_pool_thread_create( struct mphig_thread_pool_thread*       Thread_Pool_Thread,
                                 MELPHIG_OPTIONAL struct mphig_error*   Error )
{
    mphig                           ret                     = MELPHIG_FAIL;
    struct mphig_mutex_attr         mutex_attr              = MELPHIG_CONST_MPHIG_MUTEX_ATTR;
    struct mphig_thread_cond_attr   thread_cond_attr        = MELPHIG_CONST_MPHIG_THREAD_COND_ATTR;
    struct mphig_thread_attr        thread_attr             = MELPHIG_CONST_MPHIG_THREAD_ATTR;
    struct mphig_error              unlock_error            = MELPHIG_CONST_MPHIG_ERROR;
    mphig                           timed_wait_ret          = MELPHIG_FAIL;
    struct mphig_error              timed_wait_error        = MELPHIG_CONST_MPHIG_ERROR;

    // NULL checks
    if( Thread_Pool_Thread == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Thread_Pool_Thread is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    if( MELPHIG_OK == ( ret = mphig_mutex_init( &Thread_Pool_Thread->mutex,
                                                &mutex_attr,
                                                Error ) ) )
    {
        if( MELPHIG_OK == ( ret = mphig_mutex_init( &Thread_Pool_Thread->busy_mutex,
                                                    &mutex_attr,
                                                    Error ) ) )
        {
            if( MELPHIG_OK == ( ret = mphig_thread_cond_init( &Thread_Pool_Thread->thread_cond,
                                                              &thread_cond_attr,
                                                              Error ) ) )
            {
                if( MELPHIG_OK == ( ret = mphig_thread_cond_init( &Thread_Pool_Thread->ready_thread_cond,
                                                                  &thread_cond_attr,
                                                                  Error ) ) )
                {
                    // Reset
                    Thread_Pool_Thread->busy                = 0;
                    Thread_Pool_Thread->thread_cond_kind    = mphig_thread_pool_thread_cond_kind_ready;

                    if( MELPHIG_OK == ( ret = mphig_thread_create( &Thread_Pool_Thread->thread,
                                                                   &thread_attr,
                                                                   &thread_pool_thread_routine,
                                                                   Thread_Pool_Thread,
                                                                   Error ) ) )
                    {
                        if( MELPHIG_OK == mphig_mutex_lock( &Thread_Pool_Thread->mutex,
                                                            Error ) )
                        {
                            // Assume fail
                            timed_wait_ret = MELPHIG_FAIL;

                            // Poke the thread until its ready
                            while( ret == MELPHIG_OK &&
                                   timed_wait_ret == MELPHIG_FAIL )
                            {
                                // Signal alot , dont wait for long
                                if( MELPHIG_OK == ( ret = mphig_thread_cond_signal( &Thread_Pool_Thread->thread_cond,
                                                                                    Error ) ) )
                                {
                                    // TODO max number of attemps ? otherwise this might deadlock when something goes wrong
                                    if( MELPHIG_FAIL == ( timed_wait_ret = mphig_thread_cond_timed_wait( &Thread_Pool_Thread->ready_thread_cond,
                                                                                                         &Thread_Pool_Thread->mutex,
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

                            if( ret == MELPHIG_FAIL )
                            {
                                Thread_Pool_Thread->thread_cond_kind    = mphig_thread_pool_thread_cond_kind_abort;
                                mphig_thread_cond_signal( &Thread_Pool_Thread->thread_cond,
                                                          NULL );

                                mphig_thread_join( &Thread_Pool_Thread->thread,
                                                   NULL,
                                                   NULL );
                            }

                            if( MELPHIG_FAIL == mphig_mutex_unlock( &Thread_Pool_Thread->mutex,
                                                                    &unlock_error ) )
                            {
                                if( ret == MELPHIG_OK )
                                {
                                    if( Error != NULL )
                                        *Error = unlock_error;

                                    ret = MELPHIG_FAIL;
                                }
                            }
                        } // mutex locked
                    } // thread created

                    if( ret == MELPHIG_FAIL )
                    {
                        mphig_thread_cond_destroy( &Thread_Pool_Thread->ready_thread_cond,
                                                   NULL );
                    }
                } // ready_thread_cond init

                if( ret == MELPHIG_FAIL )
                {
                    mphig_thread_cond_destroy( &Thread_Pool_Thread->thread_cond,
                                               NULL );
                }
            } // thread cond init
            if( ret == MELPHIG_FAIL )
            {
                mphig_mutex_destroy( &Thread_Pool_Thread->busy_mutex,
                                     NULL );
            }
        } // busy mutex init

        if( ret == MELPHIG_FAIL )
        {
            mphig_mutex_destroy( &Thread_Pool_Thread->mutex,
                                 NULL );
        }
    } // mutex init

    return ret;
}
