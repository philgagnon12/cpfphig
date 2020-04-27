#include "cpfphig/cpfphig.h"
#include "cpfphig/thread_pool_thread.h"
#include "cpfphig/malloc.h"
#include "cpfphig/free.h"
#include "cpfphig/thread_cond_signal.h"
#include "cpfphig/thread_join.h"
#include "cpfphig/thread_cond_destroy.h"
#include "cpfphig/mutex_destroy.h"
#include "cpfphig/mutex_lock.h"
#include "cpfphig/mutex_unlock.h"

cpfphig
cpfphig_destroy_thread_pool_thread( struct cpfphig_thread_pool_thread*      Thread_Pool_Thread,
                                    CPFPHIG_OPTIONAL struct cpfphig_error*  Error )
{
    cpfphig               ret                 = CPFPHIG_FAIL;
    int                 mutex_unlocked      = 0;
    struct cpfphig_error  mutex_unlock_error  = CPFPHIG_CONST_CPFPHIG_ERROR;

    // NULL checks
    if( Thread_Pool_Thread == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Thread_Pool_Thread is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    if( CPFPHIG_FAIL == cpfphig_mutex_lock( &Thread_Pool_Thread->mutex,
                                          Error ) )
    {
        return CPFPHIG_FAIL;
    }

    Thread_Pool_Thread->thread_cond_kind    = cpfphig_thread_pool_thread_cond_kind_abort;

    if( CPFPHIG_OK == ( ret = cpfphig_thread_cond_signal( &Thread_Pool_Thread->thread_cond,
                                                        Error ) ) )
    {
        if( CPFPHIG_OK == ( ret = cpfphig_mutex_unlock( &Thread_Pool_Thread->mutex,
                                                      Error ) ) )
        {
            mutex_unlocked = 1;

            if( CPFPHIG_OK == ( ret = cpfphig_thread_join( &Thread_Pool_Thread->thread,
                                                           NULL,
                                                           Error ) ) )
            {
                if( CPFPHIG_OK == ( ret = cpfphig_thread_cond_destroy( &Thread_Pool_Thread->ready_thread_cond,
                                                                     Error ) ) )
                {
                    if( CPFPHIG_OK == ( ret = cpfphig_thread_cond_destroy( &Thread_Pool_Thread->thread_cond,
                                                                         Error ) ) )
                    {
                        if( CPFPHIG_OK == ( ret = cpfphig_mutex_destroy( &Thread_Pool_Thread->busy_mutex,
                                                                       Error ) ) )
                        {

                            if( CPFPHIG_OK == ( ret = cpfphig_mutex_destroy( &Thread_Pool_Thread->mutex,
                                                                           Error ) ) )
                            {
                                ret = CPFPHIG_OK;
                            }
                        }
                    }
                }
            } // thread join
        } // unlock
    } // signal

    if( mutex_unlocked == 0 )
    {
        if( CPFPHIG_FAIL == cpfphig_mutex_unlock( &Thread_Pool_Thread->mutex,
                                                &mutex_unlock_error ) )
        {
            if( ret == CPFPHIG_OK )
            {
                if( Error != NULL )
                    *Error = mutex_unlock_error;

                ret = CPFPHIG_FAIL;
            }
        }
    }

    return ret;
}
