#include "melphig/melphig.h"
#include "melphig/thread_pool.h"
#include "melphig/list.h"
#include "melphig/destroy_thread_pool_thread.h"
#include "melphig/free.h"
#include "melphig/mutex_destroy.h"

fphig
fphig_destroy_thread_pool( struct fphig_thread_pool*            Thread_Pool,
                           FPHIG_OPTIONAL struct fphig_error* Error )
{
    fphig                                                   ret                                         = FPHIG_OK;
    fphig                                                   shift_thread_pool_thread_ret                = FPHIG_FAIL;
    struct fphig_error                                      shift_thread_pool_thread_error              = FPHIG_CONST_MPHIG_ERROR;
    struct fphig_thread_pool_thread*                        thread_pool_thread                          = NULL;
    struct fphig_error                                      mutex_destroy_error                         = FPHIG_CONST_MPHIG_ERROR;

    // NULL checks
    if( Thread_Pool == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "Thread_Pool is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    // Assume ok
    ret = FPHIG_OK;

    while( ret == FPHIG_OK &&
           FPHIG_OK == ( shift_thread_pool_thread_ret = fphig_list_shift( &Thread_Pool->thread_pool_threads,
                                                                            &thread_pool_thread,
                                                                            &shift_thread_pool_thread_error ) ) )
    {
        if( thread_pool_thread == NULL )
        {
            if( Error != NULL )
                fphig_error_message( fphig_system_error,"thread_pool_thread is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

            ret = FPHIG_FAIL;
        }
        if( ret == FPHIG_OK )
        {
            if( FPHIG_OK == ( ret = fphig_destroy_thread_pool_thread( thread_pool_thread,
                                                                        Error ) ) )
            {
                ret = fphig_free( &thread_pool_thread,
                                  Error );
            } // thread_pool_thread destroyed
        }
    }
    if( shift_thread_pool_thread_ret == FPHIG_FAIL &&
        shift_thread_pool_thread_error.error_type == fphig_system_error )
    {
        ret = FPHIG_FAIL;
    }

    if( FPHIG_FAIL == fphig_mutex_destroy( &Thread_Pool->mutex,
                                             &mutex_destroy_error ) )
    {
        if( ret == FPHIG_OK )
        {
            if( Error != NULL )
                *Error = mutex_destroy_error;

            ret = FPHIG_FAIL;
        }
    }

    return ret;
}
