#include "melphig/melphig.h"
#include "melphig/thread_pool_task.h"
#include "melphig/thread_pool.h"
#include "melphig/thread_pool_thread.h"
#include "melphig/thread_pool_thread_create.h"
#include "melphig/mutex_lock.h"
#include "melphig/mutex_unlock.h"
#include "melphig/thread_cond_signal.h"
#include "melphig/malloc.h"
#include "melphig/free.h"
#include "melphig/list.h"


#include <unistd.h> // TODO remove

mphig
mphig_thread_pool_task( struct mphig_thread_pool*                   Thread_Pool,
                        mphig_thread_pool_thread_routine_symbol*    Routine,
                        void*                                       Routine_Arg,
                        MELPHIG_OPTIONAL struct mphig_error*        Error )
{
    mphig                                                   ret                                         = MELPHIG_OK;
    struct mphig_list_iterator                              thread_pool_threads_iterator                = MELPHIG_CONST_MPHIG_LIST_ITERATOR;
    mphig                                                   next_thread_pool_thread_ret                 = MELPHIG_FAIL;
    struct mphig_error                                      next_thread_pool_thread_error               = MELPHIG_CONST_MPHIG_ERROR;
    struct mphig_thread_pool_thread*                        thread_pool_thread                          = NULL;
    struct mphig_error                                      unlock_busy_mutex_error                     = MELPHIG_CONST_MPHIG_ERROR;
    struct mphig_thread_pool_thread*                        available_thread_pool_thread                = NULL;
    struct mphig_thread_pool_thread*                        new_thread_pool_thread                      = NULL;
    struct mphig_thread_pool_thread                         const_thread_pool_thread                    = MELPHIG_CONST_MPHIG_THREAD_POOL_THREAD;
    struct mphig_error                                      unlock_mutex_error                          = MELPHIG_CONST_MPHIG_ERROR;

    // NULL checks
    if( Thread_Pool == NULL || Routine == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Thread_Pool or Routine is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }


    if( MELPHIG_FAIL == mphig_mutex_lock( &Thread_Pool->mutex,
                                          Error ) )
    {
        return MELPHIG_FAIL;
    }

    thread_pool_threads_iterator.list           = &Thread_Pool->thread_pool_threads;
    thread_pool_threads_iterator.current_node   = NULL;

    // Assume ok
    ret = MELPHIG_OK;

    while( ret == MELPHIG_OK &&
           available_thread_pool_thread == NULL &&
           MELPHIG_OK == ( next_thread_pool_thread_ret = mphig_list_next( &thread_pool_threads_iterator,
                                                                          &thread_pool_thread,
                                                                          &next_thread_pool_thread_error ) ) )
    {
        if( MELPHIG_OK == ( ret = mphig_mutex_lock( &thread_pool_thread->busy_mutex,
                                                    Error ) ) )
        {

            if( thread_pool_thread->busy == 0 )
            {
                available_thread_pool_thread = thread_pool_thread;
            }

            // Error or not , we have to unlock
            if( MELPHIG_FAIL == mphig_mutex_unlock( &thread_pool_thread->busy_mutex,
                                                    &unlock_busy_mutex_error ) )
            {
                if( ret == MELPHIG_OK )
                {
                    if( Error != NULL )
                        *Error = unlock_busy_mutex_error;

                    ret = MELPHIG_FAIL;
                }
            }
        } // mutex locked
    }
    if( next_thread_pool_thread_ret == MELPHIG_FAIL &&
        next_thread_pool_thread_error.error_type == mphig_system_error )
    {
        ret = MELPHIG_FAIL;
    }

    if( ret == MELPHIG_OK )
    {
        // No threads were available, add to our pool
        if( available_thread_pool_thread == NULL )
        {
            if( MELPHIG_OK == ( ret = mphig_malloc( sizeof( struct mphig_thread_pool_thread ),
                                                    &new_thread_pool_thread,
                                                    Error ) ) )
            {
                // Reset
                *new_thread_pool_thread = const_thread_pool_thread;

                if( MELPHIG_OK == ( ret = mphig_thread_pool_thread_create( new_thread_pool_thread,
                                                                           Error ) ) )
                {
                    available_thread_pool_thread = new_thread_pool_thread;

                    if( MELPHIG_OK == ( ret = mphig_list_push( &Thread_Pool->thread_pool_threads,
                                                               new_thread_pool_thread,
                                                               Error ) ) )
                    {
                        ret = MELPHIG_OK;
                    }
                }
            }
        } // available thread is NULL

        if( MELPHIG_OK == mphig_mutex_lock( &available_thread_pool_thread->mutex,
                                              Error ) )
        {
            if( MELPHIG_OK == ( ret = mphig_mutex_lock( &available_thread_pool_thread->busy_mutex,
                                                        Error ) ) )
            {
                available_thread_pool_thread->busy = 1;

                available_thread_pool_thread->routine             = Routine;
                available_thread_pool_thread->routine_arg         = Routine_Arg;
                available_thread_pool_thread->thread_cond_kind    = mphig_thread_pool_thread_cond_kind_routine;

                ret = mphig_thread_cond_signal( &available_thread_pool_thread->thread_cond,
                                                Error );

                if( MELPHIG_FAIL == mphig_mutex_unlock( &available_thread_pool_thread->busy_mutex,
                                                        &unlock_busy_mutex_error ) )
                {
                    if( ret == MELPHIG_OK )
                    {
                        if( Error != NULL )
                            *Error = unlock_busy_mutex_error;

                        ret = MELPHIG_FAIL;
                    }
                }
            } // busy_mutex locked

            if( MELPHIG_FAIL == mphig_mutex_unlock( &available_thread_pool_thread->mutex,
                                                    &unlock_mutex_error ) )
            {
                if( ret == MELPHIG_OK )
                {
                    if( Error != NULL )
                        *Error = unlock_mutex_error;

                    ret = MELPHIG_FAIL;
                }
            }
        } // available_thread_pool_thread->mutex locked

    } // OK after looping thread_pool_threads

    if( MELPHIG_FAIL == mphig_mutex_unlock( &Thread_Pool->mutex,
                                            &unlock_mutex_error ) )
    {
        if( ret == MELPHIG_OK )
        {
            if( Error != NULL )
                *Error = unlock_mutex_error;

            ret = MELPHIG_FAIL;
        }
    }

    if( ret == MELPHIG_FAIL )
    {
        mphig_free( &new_thread_pool_thread, NULL );
    }

    return ret;
}
