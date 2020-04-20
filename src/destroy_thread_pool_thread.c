#include "melphig/melphig.h"
#include "melphig/thread_pool_thread.h"
#include "melphig/malloc.h"
#include "melphig/free.h"
#include "melphig/thread_cond_signal.h"
#include "melphig/thread_join.h"
#include "melphig/thread_cond_destroy.h"
#include "melphig/mutex_destroy.h"
#include "melphig/mutex_lock.h"
#include "melphig/mutex_unlock.h"

mphig
mphig_destroy_thread_pool_thread( struct mphig_thread_pool_thread*      Thread_Pool_Thread,
                                  MELPHIG_OPTIONAL struct mphig_error*  Error )
{
    mphig               ret                 = MELPHIG_FAIL;
    int                 mutex_unlocked      = 0;
    struct mphig_error  mutex_unlock_error  = MELPHIG_CONST_MPHIG_ERROR;

    // NULL checks
    if( Thread_Pool_Thread == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Thread_Pool_Thread is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    if( MELPHIG_FAIL == mphig_mutex_lock( &Thread_Pool_Thread->mutex,
                                          Error ) )
    {
        return MELPHIG_FAIL;
    }

    Thread_Pool_Thread->thread_cond_kind    = mphig_thread_pool_thread_cond_kind_abort;

    if( MELPHIG_OK == ( ret = mphig_thread_cond_signal( &Thread_Pool_Thread->thread_cond,
                                                        Error ) ) )
    {
        if( MELPHIG_OK == ( ret = mphig_mutex_unlock( &Thread_Pool_Thread->mutex,
                                                      Error ) ) )
        {
            mutex_unlocked = 1;

            if( MELPHIG_OK == ( ret = mphig_thread_join( &Thread_Pool_Thread->thread,
                                                         NULL,
                                                         Error ) ) )
            {
                if( MELPHIG_OK == ( ret = mphig_thread_cond_destroy( &Thread_Pool_Thread->ready_thread_cond,
                                                                     Error ) ) )
                {
                    if( MELPHIG_OK == ( ret = mphig_thread_cond_destroy( &Thread_Pool_Thread->thread_cond,
                                                                         Error ) ) )
                    {
                        if( MELPHIG_OK == ( ret = mphig_mutex_destroy( &Thread_Pool_Thread->busy_mutex,
                                                                       Error ) ) )
                        {

                            if( MELPHIG_OK == ( ret = mphig_mutex_destroy( &Thread_Pool_Thread->mutex,
                                                                           Error ) ) )
                            {
                                ret = MELPHIG_OK;
                            }
                        }
                    }
                }
            } // thread join
        } // unlock
    } // signal

    if( mutex_unlocked == 0 )
    {
        if( MELPHIG_FAIL == mphig_mutex_unlock( &Thread_Pool_Thread->mutex,
                                                &mutex_unlock_error ) )
        {
            if( ret == MELPHIG_OK )
            {
                if( Error != NULL )
                    *Error = mutex_unlock_error;

                ret = MELPHIG_FAIL;
            }
        }
    }

    return ret;
}
