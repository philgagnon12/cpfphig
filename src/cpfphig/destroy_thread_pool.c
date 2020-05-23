#include "cpfphig/cpfphig.h"
#include "cpfphig/thread_pool.h"
#include "cpfphig/list.h"
#include "cpfphig/destroy_thread_pool_thread.h"
#include "cpfphig/free.h"
#include "cpfphig/mutex_destroy.h"

cpfphig
cpfphig_destroy_thread_pool( struct cpfphig_thread_pool*            Thread_Pool,
                             CPFPHIG_OPTIONAL struct cpfphig_error* Error )
{
    cpfphig                             ret                                         = CPFPHIG_OK;
    cpfphig                             shift_thread_pool_thread_ret                = CPFPHIG_FAIL;
    struct cpfphig_error                shift_thread_pool_thread_error              = CPFPHIG_CONST_CPFPHIG_ERROR;
    struct cpfphig_thread_pool_thread*  thread_pool_thread                          = NULL;
    struct cpfphig_error                mutex_destroy_error                         = CPFPHIG_CONST_CPFPHIG_ERROR;

    // NULL checks
    if( Thread_Pool == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Thread_Pool is NULL", Error );

        return CPFPHIG_FAIL;
    }

    // Assume ok
    ret = CPFPHIG_OK;

    shift_thread_pool_thread_ret                = CPFPHIG_OK;
    shift_thread_pool_thread_error.error_type   = cpfphig_ok;

    while( ret == CPFPHIG_OK &&
           CPFPHIG_OK == ( shift_thread_pool_thread_ret = cpfphig_list_shift( &Thread_Pool->thread_pool_threads,
                                                                              &thread_pool_thread,
                                                                              &shift_thread_pool_thread_error ) ) )
    {
        if( thread_pool_thread == NULL )
        {
            if( Error != NULL )
                cpfphig_error_message( cpfphig_system_error,"thread_pool_thread is NULL", Error );

            ret = CPFPHIG_FAIL;
        }
        if( ret == CPFPHIG_OK )
        {
            if( CPFPHIG_OK == ( ret = cpfphig_destroy_thread_pool_thread( thread_pool_thread,
                                                                          Error ) ) )
            {
                ret = cpfphig_free( &thread_pool_thread,
                                    Error );
            } // thread_pool_thread destroyed
        }
    }
    if( shift_thread_pool_thread_error.error_type   == cpfphig_system_error  &&
        shift_thread_pool_thread_ret                == CPFPHIG_FAIL )
    {
        ret = CPFPHIG_FAIL;
    }

    if( CPFPHIG_FAIL == cpfphig_mutex_destroy( &Thread_Pool->mutex,
                                               &mutex_destroy_error ) )
    {
        if( ret == CPFPHIG_OK )
        {
            if( Error != NULL )
                *Error = mutex_destroy_error;

            ret = CPFPHIG_FAIL;
        }
    }

    return ret;
}
