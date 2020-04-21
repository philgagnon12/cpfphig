#include "cpfphig/cpfphig.h"
#include "cpfphig/thread_pool_task.h"
#include "cpfphig/thread_pool.h"
#include "cpfphig/thread_pool_thread.h"
#include "cpfphig/thread_pool_thread_create.h"
#include "cpfphig/mutex_lock.h"
#include "cpfphig/mutex_unlock.h"
#include "cpfphig/thread_cond_signal.h"
#include "cpfphig/malloc.h"
#include "cpfphig/free.h"
#include "cpfphig/list.h"


#include <unistd.h> // TODO remove

cpfphig
cpfphig_thread_pool_task( struct cpfphig_thread_pool*                   Thread_Pool,
                        cpfphig_thread_pool_thread_routine_symbol*    Routine,
                        void*                                       Routine_Arg,
                        CPFPHIG_OPTIONAL struct cpfphig_error*        Error )
{
    cpfphig                                                   ret                                         = CPFPHIG_OK;
    struct cpfphig_list_iterator                              thread_pool_threads_iterator                = CPFPHIG_CONST_CPFPHIG_LIST_ITERATOR;
    cpfphig                                                   next_thread_pool_thread_ret                 = CPFPHIG_FAIL;
    struct cpfphig_error                                      next_thread_pool_thread_error               = CPFPHIG_CONST_CPFPHIG_ERROR;
    struct cpfphig_thread_pool_thread*                        thread_pool_thread                          = NULL;
    struct cpfphig_error                                      unlock_busy_mutex_error                     = CPFPHIG_CONST_CPFPHIG_ERROR;
    struct cpfphig_thread_pool_thread*                        available_thread_pool_thread                = NULL;
    struct cpfphig_thread_pool_thread*                        new_thread_pool_thread                      = NULL;
    struct cpfphig_thread_pool_thread                         const_thread_pool_thread                    = CPFPHIG_CONST_CPFPHIG_THREAD_POOL_THREAD;
    struct cpfphig_error                                      unlock_mutex_error                          = CPFPHIG_CONST_CPFPHIG_ERROR;

    // NULL checks
    if( Thread_Pool == NULL || Routine == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Thread_Pool or Routine is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }


    if( CPFPHIG_FAIL == cpfphig_mutex_lock( &Thread_Pool->mutex,
                                          Error ) )
    {
        return CPFPHIG_FAIL;
    }

    thread_pool_threads_iterator.list           = &Thread_Pool->thread_pool_threads;
    thread_pool_threads_iterator.current_node   = NULL;

    // Assume ok
    ret = CPFPHIG_OK;

    while( ret == CPFPHIG_OK &&
           available_thread_pool_thread == NULL &&
           CPFPHIG_OK == ( next_thread_pool_thread_ret = cpfphig_list_next( &thread_pool_threads_iterator,
                                                                          &thread_pool_thread,
                                                                          &next_thread_pool_thread_error ) ) )
    {
        if( CPFPHIG_OK == ( ret = cpfphig_mutex_lock( &thread_pool_thread->busy_mutex,
                                                    Error ) ) )
        {

            if( thread_pool_thread->busy == 0 )
            {
                available_thread_pool_thread = thread_pool_thread;
            }

            // Error or not , we have to unlock
            if( CPFPHIG_FAIL == cpfphig_mutex_unlock( &thread_pool_thread->busy_mutex,
                                                    &unlock_busy_mutex_error ) )
            {
                if( ret == CPFPHIG_OK )
                {
                    if( Error != NULL )
                        *Error = unlock_busy_mutex_error;

                    ret = CPFPHIG_FAIL;
                }
            }
        } // mutex locked
    }
    if( next_thread_pool_thread_ret == CPFPHIG_FAIL &&
        next_thread_pool_thread_error.error_type == cpfphig_system_error )
    {
        ret = CPFPHIG_FAIL;
    }

    if( ret == CPFPHIG_OK )
    {
        // No threads were available, add to our pool
        if( available_thread_pool_thread == NULL )
        {
            if( CPFPHIG_OK == ( ret = cpfphig_malloc( sizeof( struct cpfphig_thread_pool_thread ),
                                                    &new_thread_pool_thread,
                                                    Error ) ) )
            {
                // Reset
                *new_thread_pool_thread = const_thread_pool_thread;

                if( CPFPHIG_OK == ( ret = cpfphig_thread_pool_thread_create( new_thread_pool_thread,
                                                                           Error ) ) )
                {
                    available_thread_pool_thread = new_thread_pool_thread;

                    if( CPFPHIG_OK == ( ret = cpfphig_list_push( &Thread_Pool->thread_pool_threads,
                                                               new_thread_pool_thread,
                                                               Error ) ) )
                    {
                        ret = CPFPHIG_OK;
                    }
                }
            }
        } // available thread is NULL

        if( CPFPHIG_OK == cpfphig_mutex_lock( &available_thread_pool_thread->mutex,
                                              Error ) )
        {
            if( CPFPHIG_OK == ( ret = cpfphig_mutex_lock( &available_thread_pool_thread->busy_mutex,
                                                        Error ) ) )
            {
                available_thread_pool_thread->busy = 1;

                available_thread_pool_thread->routine             = Routine;
                available_thread_pool_thread->routine_arg         = Routine_Arg;
                available_thread_pool_thread->thread_cond_kind    = cpfphig_thread_pool_thread_cond_kind_routine;

                ret = cpfphig_thread_cond_signal( &available_thread_pool_thread->thread_cond,
                                                Error );

                if( CPFPHIG_FAIL == cpfphig_mutex_unlock( &available_thread_pool_thread->busy_mutex,
                                                        &unlock_busy_mutex_error ) )
                {
                    if( ret == CPFPHIG_OK )
                    {
                        if( Error != NULL )
                            *Error = unlock_busy_mutex_error;

                        ret = CPFPHIG_FAIL;
                    }
                }
            } // busy_mutex locked

            if( CPFPHIG_FAIL == cpfphig_mutex_unlock( &available_thread_pool_thread->mutex,
                                                    &unlock_mutex_error ) )
            {
                if( ret == CPFPHIG_OK )
                {
                    if( Error != NULL )
                        *Error = unlock_mutex_error;

                    ret = CPFPHIG_FAIL;
                }
            }
        } // available_thread_pool_thread->mutex locked

    } // OK after looping thread_pool_threads

    if( CPFPHIG_FAIL == cpfphig_mutex_unlock( &Thread_Pool->mutex,
                                            &unlock_mutex_error ) )
    {
        if( ret == CPFPHIG_OK )
        {
            if( Error != NULL )
                *Error = unlock_mutex_error;

            ret = CPFPHIG_FAIL;
        }
    }

    if( ret == CPFPHIG_FAIL )
    {
        cpfphig_free( &new_thread_pool_thread, NULL );
    }

    return ret;
}
