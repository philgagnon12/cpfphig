#include "melphig/melphig.h"
#include "melphig/thread_pool.h"
#include "melphig/list.h"
#include "melphig/destroy_thread_pool_thread.h"
#include "melphig/free.h"
#include "melphig/mutex_destroy.h"

mphig
mphig_destroy_thread_pool( struct mphig_thread_pool*            Thread_Pool,
                           MELPHIG_OPTIONAL struct mphig_error* Error )
{
    mphig                                                   ret                                         = MELPHIG_OK;
    mphig                                                   shift_thread_pool_thread_ret                = MELPHIG_FAIL;
    struct mphig_error                                      shift_thread_pool_thread_error              = MELPHIG_CONST_MPHIG_ERROR;
    struct mphig_thread_pool_thread*                        thread_pool_thread                          = NULL;
    struct mphig_error                                      mutex_destroy_error                         = MELPHIG_CONST_MPHIG_ERROR;

    // NULL checks
    if( Thread_Pool == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Thread_Pool is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    // Assume ok
    ret = MELPHIG_OK;

    while( ret == MELPHIG_OK &&
           MELPHIG_OK == ( shift_thread_pool_thread_ret = mphig_list_shift( &Thread_Pool->thread_pool_threads,
                                                                            &thread_pool_thread,
                                                                            &shift_thread_pool_thread_error ) ) )
    {
        if( thread_pool_thread == NULL )
        {
            if( Error != NULL )
                mphig_error_message( mphig_system_error,"thread_pool_thread is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

            ret = MELPHIG_FAIL;
        }
        if( ret == MELPHIG_OK )
        {
            if( MELPHIG_OK == ( ret = mphig_destroy_thread_pool_thread( thread_pool_thread,
                                                                        Error ) ) )
            {
                ret = mphig_free( &thread_pool_thread,
                                  Error );
            } // thread_pool_thread destroyed
        }
    }
    if( shift_thread_pool_thread_ret == MELPHIG_FAIL &&
        shift_thread_pool_thread_error.error_type == mphig_system_error )
    {
        ret = MELPHIG_FAIL;
    }

    if( MELPHIG_FAIL == mphig_mutex_destroy( &Thread_Pool->mutex,
                                             &mutex_destroy_error ) )
    {
        if( ret == MELPHIG_OK )
        {
            if( Error != NULL )
                *Error = mutex_destroy_error;

            ret = MELPHIG_FAIL;
        }
    }

    return ret;
}
