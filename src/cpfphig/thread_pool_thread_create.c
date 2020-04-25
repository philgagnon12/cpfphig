#include "cpfphig/cpfphig.h"
#include "cpfphig/thread_pool_thread.h"
#include "cpfphig/thread_pool_thread_create.h"
#include "cpfphig/thread_pool.h"
#include "cpfphig/malloc.h"
#include "cpfphig/free.h"
#include "cpfphig/mutex.h"
#include "cpfphig/mutex_init.h"
#include "cpfphig/mutex_destroy.h"
#include "cpfphig/mutex_attr.h"
#include "cpfphig/mutex_lock.h"
#include "cpfphig/mutex_unlock.h"
#include "cpfphig/thread_create.h"
#include "cpfphig/thread_join.h"
#include "cpfphig/thread_attr.h"
#include "cpfphig/thread_cond_init.h"
#include "cpfphig/thread_cond_attr.h"
#include "cpfphig/thread_cond_destroy.h"
#include "cpfphig/thread_cond_signal.h"
#include "cpfphig/thread_cond_wait.h"
#include "cpfphig/thread_cond_timed_wait.h"
#include "cpfphig/assert.h"

static
int
thread_pool_thread_routine( void* Thread_Pool_Thread )
{
    struct cpfphig_thread_pool_thread*    thread_pool_thread  = NULL;
    int                                 abort               = 0;

    thread_pool_thread = (struct cpfphig_thread_pool_thread*)Thread_Pool_Thread;
    cpfphig_assert( thread_pool_thread != NULL, "Thread_Pool_Thread is NULL", __FILE__, __FUNCTION__, __LINE__ );

    cpfphig_assert( CPFPHIG_OK == cpfphig_mutex_lock( &thread_pool_thread->mutex,
                                                  NULL ),
                  "cpfphig_mutex_lock failed",
                  __FILE__,
                  __FUNCTION__,
                  __LINE__ );

    while( abort == 0 )
    {
        cpfphig_assert( CPFPHIG_OK == cpfphig_thread_cond_wait( &thread_pool_thread->thread_cond,
                                                            &thread_pool_thread->mutex,
                                                            NULL ),
                      "cpfphig_thread_cond_wait failed",
                      __FILE__,
                      __FUNCTION__,
                      __LINE__ );

        switch( thread_pool_thread->thread_cond_kind )
        {
            case cpfphig_thread_pool_thread_cond_kind_ready:
                cpfphig_assert( CPFPHIG_OK == cpfphig_thread_cond_signal( &thread_pool_thread->ready_thread_cond,
                                                                      NULL ),
                  "cpfphig_thread_cond_signal failed",
                  __FILE__,
                  __FUNCTION__,
                  __LINE__ );
                break;
            case cpfphig_thread_pool_thread_cond_kind_routine:
                cpfphig_assert( thread_pool_thread->routine != NULL, "thread_pool_thread->routine is NULL", __FILE__, __FUNCTION__, __LINE__ );

                thread_pool_thread->routine( thread_pool_thread->routine_arg );

                cpfphig_assert( CPFPHIG_OK == cpfphig_mutex_lock( &thread_pool_thread->busy_mutex,
                                                              NULL ),
                          "cpfphig_mutex_lock failed",
                          __FILE__,
                          __FUNCTION__,
                          __LINE__ );

                // Not busy anymore
                thread_pool_thread->busy = 0;

                cpfphig_assert( CPFPHIG_OK == cpfphig_mutex_unlock( &thread_pool_thread->busy_mutex,
                                                                NULL ),
                          "cpfphig_mutex_unlock failed",
                          __FILE__,
                          __FUNCTION__,
                          __LINE__ );
                break;
            case cpfphig_thread_pool_thread_cond_kind_abort:
                abort = 1;
                break;
            default:
                cpfphig_assert( thread_pool_thread->thread_cond_kind == cpfphig_thread_pool_thread_cond_kind_ready ||
                              thread_pool_thread->thread_cond_kind == cpfphig_thread_pool_thread_cond_kind_routine ||
                              thread_pool_thread->thread_cond_kind == cpfphig_thread_pool_thread_cond_kind_abort, 
                              "thread_pool_thread->thread_cond_kind not supported",
                              __FILE__,
                              __FUNCTION__,
                              __LINE__ );
        } // switch
    } // while

    cpfphig_assert( CPFPHIG_OK == cpfphig_mutex_unlock( &thread_pool_thread->mutex,
                                                    NULL ),
                  "cpfphig_mutex_unlock failed",
                  __FILE__,
                  __FUNCTION__,
                  __LINE__ );


    return 0;
}

cpfphig
cpfphig_thread_pool_thread_create( struct cpfphig_thread_pool_thread*       Thread_Pool_Thread,
                                   CPFPHIG_OPTIONAL struct cpfphig_error*   Error )
{
    cpfphig                           ret                     = CPFPHIG_FAIL;
    struct cpfphig_mutex_attr         mutex_attr              = CPFPHIG_CONST_CPFPHIG_MUTEX_ATTR;
    struct cpfphig_thread_cond_attr   thread_cond_attr        = CPFPHIG_CONST_CPFPHIG_THREAD_COND_ATTR;
    struct cpfphig_thread_attr        thread_attr             = CPFPHIG_CONST_CPFPHIG_THREAD_ATTR;
    struct cpfphig_error              unlock_error            = CPFPHIG_CONST_CPFPHIG_ERROR;
    cpfphig                           timed_wait_ret          = CPFPHIG_FAIL;
    struct cpfphig_error              timed_wait_error        = CPFPHIG_CONST_CPFPHIG_ERROR;

    // NULL checks
    if( Thread_Pool_Thread == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Thread_Pool_Thread is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    if( CPFPHIG_OK == ( ret = cpfphig_mutex_init( &Thread_Pool_Thread->mutex,
                                                &mutex_attr,
                                                Error ) ) )
    {
        if( CPFPHIG_OK == ( ret = cpfphig_mutex_init( &Thread_Pool_Thread->busy_mutex,
                                                    &mutex_attr,
                                                    Error ) ) )
        {
            if( CPFPHIG_OK == ( ret = cpfphig_thread_cond_init( &Thread_Pool_Thread->thread_cond,
                                                              &thread_cond_attr,
                                                              Error ) ) )
            {
                if( CPFPHIG_OK == ( ret = cpfphig_thread_cond_init( &Thread_Pool_Thread->ready_thread_cond,
                                                                  &thread_cond_attr,
                                                                  Error ) ) )
                {
                    // Reset
                    Thread_Pool_Thread->busy                = 0;
                    Thread_Pool_Thread->thread_cond_kind    = cpfphig_thread_pool_thread_cond_kind_ready;

                    if( CPFPHIG_OK == ( ret = cpfphig_thread_create( &Thread_Pool_Thread->thread,
                                                                   &thread_attr,
                                                                   &thread_pool_thread_routine,
                                                                   Thread_Pool_Thread,
                                                                   Error ) ) )
                    {
                        if( CPFPHIG_OK == cpfphig_mutex_lock( &Thread_Pool_Thread->mutex,
                                                            Error ) )
                        {
                            // Assume fail
                            timed_wait_ret = CPFPHIG_FAIL;

                            // Poke the thread until its ready
                            while( ret == CPFPHIG_OK &&
                                   timed_wait_ret == CPFPHIG_FAIL )
                            {
                                // Signal alot , dont wait for long
                                if( CPFPHIG_OK == ( ret = cpfphig_thread_cond_signal( &Thread_Pool_Thread->thread_cond,
                                                                                    Error ) ) )
                                {
                                    // TODO max number of attemps ? otherwise this might deadlock when something goes wrong
                                    if( CPFPHIG_FAIL == ( timed_wait_ret = cpfphig_thread_cond_timed_wait( &Thread_Pool_Thread->ready_thread_cond,
                                                                                                         &Thread_Pool_Thread->mutex,
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

                            if( ret == CPFPHIG_FAIL )
                            {
                                Thread_Pool_Thread->thread_cond_kind    = cpfphig_thread_pool_thread_cond_kind_abort;
                                cpfphig_thread_cond_signal( &Thread_Pool_Thread->thread_cond,
                                                          NULL );

                                cpfphig_thread_join( &Thread_Pool_Thread->thread,
                                                   NULL,
                                                   NULL );
                            }

                            if( CPFPHIG_FAIL == cpfphig_mutex_unlock( &Thread_Pool_Thread->mutex,
                                                                    &unlock_error ) )
                            {
                                if( ret == CPFPHIG_OK )
                                {
                                    if( Error != NULL )
                                        *Error = unlock_error;

                                    ret = CPFPHIG_FAIL;
                                }
                            }
                        } // mutex locked
                    } // thread created

                    if( ret == CPFPHIG_FAIL )
                    {
                        cpfphig_thread_cond_destroy( &Thread_Pool_Thread->ready_thread_cond,
                                                   NULL );
                    }
                } // ready_thread_cond init

                if( ret == CPFPHIG_FAIL )
                {
                    cpfphig_thread_cond_destroy( &Thread_Pool_Thread->thread_cond,
                                               NULL );
                }
            } // thread cond init
            if( ret == CPFPHIG_FAIL )
            {
                cpfphig_mutex_destroy( &Thread_Pool_Thread->busy_mutex,
                                     NULL );
            }
        } // busy mutex init

        if( ret == CPFPHIG_FAIL )
        {
            cpfphig_mutex_destroy( &Thread_Pool_Thread->mutex,
                                 NULL );
        }
    } // mutex init

    return ret;
}
