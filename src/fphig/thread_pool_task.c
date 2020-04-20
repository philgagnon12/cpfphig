#include "fphig/fphig.h"
#include "fphig/thread_pool_task.h"
#include "fphig/thread_pool.h"
#include "fphig/thread_pool_thread.h"
#include "fphig/thread_pool_thread_create.h"
#include "fphig/mutex_lock.h"
#include "fphig/mutex_unlock.h"
#include "fphig/thread_cond_signal.h"
#include "fphig/malloc.h"
#include "fphig/free.h"
#include "fphig/list.h"


#include <unistd.h> // TODO remove

fphig
fphig_thread_pool_task( struct fphig_thread_pool*                   Thread_Pool,
                        fphig_thread_pool_thread_routine_symbol*    Routine,
                        void*                                       Routine_Arg,
                        FPHIG_OPTIONAL struct fphig_error*        Error )
{
    fphig                                                   ret                                         = FPHIG_OK;
    struct fphig_list_iterator                              thread_pool_threads_iterator                = FPHIG_CONST_FPHIG_LIST_ITERATOR;
    fphig                                                   next_thread_pool_thread_ret                 = FPHIG_FAIL;
    struct fphig_error                                      next_thread_pool_thread_error               = FPHIG_CONST_FPHIG_ERROR;
    struct fphig_thread_pool_thread*                        thread_pool_thread                          = NULL;
    struct fphig_error                                      unlock_busy_mutex_error                     = FPHIG_CONST_FPHIG_ERROR;
    struct fphig_thread_pool_thread*                        available_thread_pool_thread                = NULL;
    struct fphig_thread_pool_thread*                        new_thread_pool_thread                      = NULL;
    struct fphig_thread_pool_thread                         const_thread_pool_thread                    = FPHIG_CONST_FPHIG_THREAD_POOL_THREAD;
    struct fphig_error                                      unlock_mutex_error                          = FPHIG_CONST_FPHIG_ERROR;

    // NULL checks
    if( Thread_Pool == NULL || Routine == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "Thread_Pool or Routine is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }


    if( FPHIG_FAIL == fphig_mutex_lock( &Thread_Pool->mutex,
                                          Error ) )
    {
        return FPHIG_FAIL;
    }

    thread_pool_threads_iterator.list           = &Thread_Pool->thread_pool_threads;
    thread_pool_threads_iterator.current_node   = NULL;

    // Assume ok
    ret = FPHIG_OK;

    while( ret == FPHIG_OK &&
           available_thread_pool_thread == NULL &&
           FPHIG_OK == ( next_thread_pool_thread_ret = fphig_list_next( &thread_pool_threads_iterator,
                                                                          &thread_pool_thread,
                                                                          &next_thread_pool_thread_error ) ) )
    {
        if( FPHIG_OK == ( ret = fphig_mutex_lock( &thread_pool_thread->busy_mutex,
                                                    Error ) ) )
        {

            if( thread_pool_thread->busy == 0 )
            {
                available_thread_pool_thread = thread_pool_thread;
            }

            // Error or not , we have to unlock
            if( FPHIG_FAIL == fphig_mutex_unlock( &thread_pool_thread->busy_mutex,
                                                    &unlock_busy_mutex_error ) )
            {
                if( ret == FPHIG_OK )
                {
                    if( Error != NULL )
                        *Error = unlock_busy_mutex_error;

                    ret = FPHIG_FAIL;
                }
            }
        } // mutex locked
    }
    if( next_thread_pool_thread_ret == FPHIG_FAIL &&
        next_thread_pool_thread_error.error_type == fphig_system_error )
    {
        ret = FPHIG_FAIL;
    }

    if( ret == FPHIG_OK )
    {
        // No threads were available, add to our pool
        if( available_thread_pool_thread == NULL )
        {
            if( FPHIG_OK == ( ret = fphig_malloc( sizeof( struct fphig_thread_pool_thread ),
                                                    &new_thread_pool_thread,
                                                    Error ) ) )
            {
                // Reset
                *new_thread_pool_thread = const_thread_pool_thread;

                if( FPHIG_OK == ( ret = fphig_thread_pool_thread_create( new_thread_pool_thread,
                                                                           Error ) ) )
                {
                    available_thread_pool_thread = new_thread_pool_thread;

                    if( FPHIG_OK == ( ret = fphig_list_push( &Thread_Pool->thread_pool_threads,
                                                               new_thread_pool_thread,
                                                               Error ) ) )
                    {
                        ret = FPHIG_OK;
                    }
                }
            }
        } // available thread is NULL

        if( FPHIG_OK == fphig_mutex_lock( &available_thread_pool_thread->mutex,
                                              Error ) )
        {
            if( FPHIG_OK == ( ret = fphig_mutex_lock( &available_thread_pool_thread->busy_mutex,
                                                        Error ) ) )
            {
                available_thread_pool_thread->busy = 1;

                available_thread_pool_thread->routine             = Routine;
                available_thread_pool_thread->routine_arg         = Routine_Arg;
                available_thread_pool_thread->thread_cond_kind    = fphig_thread_pool_thread_cond_kind_routine;

                ret = fphig_thread_cond_signal( &available_thread_pool_thread->thread_cond,
                                                Error );

                if( FPHIG_FAIL == fphig_mutex_unlock( &available_thread_pool_thread->busy_mutex,
                                                        &unlock_busy_mutex_error ) )
                {
                    if( ret == FPHIG_OK )
                    {
                        if( Error != NULL )
                            *Error = unlock_busy_mutex_error;

                        ret = FPHIG_FAIL;
                    }
                }
            } // busy_mutex locked

            if( FPHIG_FAIL == fphig_mutex_unlock( &available_thread_pool_thread->mutex,
                                                    &unlock_mutex_error ) )
            {
                if( ret == FPHIG_OK )
                {
                    if( Error != NULL )
                        *Error = unlock_mutex_error;

                    ret = FPHIG_FAIL;
                }
            }
        } // available_thread_pool_thread->mutex locked

    } // OK after looping thread_pool_threads

    if( FPHIG_FAIL == fphig_mutex_unlock( &Thread_Pool->mutex,
                                            &unlock_mutex_error ) )
    {
        if( ret == FPHIG_OK )
        {
            if( Error != NULL )
                *Error = unlock_mutex_error;

            ret = FPHIG_FAIL;
        }
    }

    if( ret == FPHIG_FAIL )
    {
        fphig_free( &new_thread_pool_thread, NULL );
    }

    return ret;
}
