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

fphig
fphig_destroy_thread_pool_thread( struct fphig_thread_pool_thread*      Thread_Pool_Thread,
                                  FPHIG_OPTIONAL struct fphig_error*  Error )
{
    fphig               ret                 = FPHIG_FAIL;
    int                 mutex_unlocked      = 0;
    struct fphig_error  mutex_unlock_error  = FPHIG_CONST_MPHIG_ERROR;

    // NULL checks
    if( Thread_Pool_Thread == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "Thread_Pool_Thread is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    if( FPHIG_FAIL == fphig_mutex_lock( &Thread_Pool_Thread->mutex,
                                          Error ) )
    {
        return FPHIG_FAIL;
    }

    Thread_Pool_Thread->thread_cond_kind    = fphig_thread_pool_thread_cond_kind_abort;

    if( FPHIG_OK == ( ret = fphig_thread_cond_signal( &Thread_Pool_Thread->thread_cond,
                                                        Error ) ) )
    {
        if( FPHIG_OK == ( ret = fphig_mutex_unlock( &Thread_Pool_Thread->mutex,
                                                      Error ) ) )
        {
            mutex_unlocked = 1;

            if( FPHIG_OK == ( ret = fphig_thread_join( &Thread_Pool_Thread->thread,
                                                         NULL,
                                                         Error ) ) )
            {
                if( FPHIG_OK == ( ret = fphig_thread_cond_destroy( &Thread_Pool_Thread->ready_thread_cond,
                                                                     Error ) ) )
                {
                    if( FPHIG_OK == ( ret = fphig_thread_cond_destroy( &Thread_Pool_Thread->thread_cond,
                                                                         Error ) ) )
                    {
                        if( FPHIG_OK == ( ret = fphig_mutex_destroy( &Thread_Pool_Thread->busy_mutex,
                                                                       Error ) ) )
                        {

                            if( FPHIG_OK == ( ret = fphig_mutex_destroy( &Thread_Pool_Thread->mutex,
                                                                           Error ) ) )
                            {
                                ret = FPHIG_OK;
                            }
                        }
                    }
                }
            } // thread join
        } // unlock
    } // signal

    if( mutex_unlocked == 0 )
    {
        if( FPHIG_FAIL == fphig_mutex_unlock( &Thread_Pool_Thread->mutex,
                                                &mutex_unlock_error ) )
        {
            if( ret == FPHIG_OK )
            {
                if( Error != NULL )
                    *Error = mutex_unlock_error;

                ret = FPHIG_FAIL;
            }
        }
    }

    return ret;
}
