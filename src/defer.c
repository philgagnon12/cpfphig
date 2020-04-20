#include "melphig/melphig.h"
#include "melphig/defer.h"
#include "melphig/thread_pool.h"
#include "melphig/malloc.h"
#include "melphig/thread_pool_task.h"
#include "melphig/free.h"
#include "melphig/sleep.h"
#include "melphig/assert.h"

static
void
defer_routine( void* Arg )
{
    struct mphig_deferred* deferred = NULL;

    mphig_assert( NULL != Arg,
                  "Arg is NULL",
                  __FILE__,
                  __FUNCTION__,
                  __LINE__ );

    deferred = Arg;

    mphig_assert( MELPHIG_OK == mphig_sleep( deferred->delay_milliseconds, NULL ),
                  "mphig_sleep failed",
                  __FILE__,
                  __FUNCTION__,
                  __LINE__ );

    deferred->routine( deferred->routine_arg );
}

mphig
mphig_defer( mphig_defer_routine_symbol*            Routine,
             void*                                  Routine_Arg,
             int                                    Delay_Milliseconds,
             struct mphig_thread_pool*              Thread_Pool,
             MELPHIG_OPTIONAL struct mphig_error*   Error )
{
    mphig                               ret             = MELPHIG_FAIL;
    struct mphig_deferred*              deferred        = NULL;
    static const struct mphig_deferred  const_deferred  = MELPHIG_CONST_MPHIG_DEFERRED;

    // NULL checks
    if( Routine == NULL || Thread_Pool == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Routine or Thread_Pool is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    if( MELPHIG_FAIL == mphig_malloc( sizeof( struct mphig_deferred ),
                                      &deferred,
                                      Error ) )
    {
        return MELPHIG_FAIL;
    }

    *deferred = const_deferred;

    deferred->routine               = Routine;
    deferred->routine_arg           = Routine_Arg;
    deferred->delay_milliseconds    = Delay_Milliseconds;

    if( MELPHIG_OK == ( ret = mphig_thread_pool_task( Thread_Pool,
                                                      &defer_routine,
                                                      deferred,
                                                      Error ) ) )
    {
        ret = MELPHIG_OK;
    }

    if( ret == MELPHIG_FAIL )
    {
        mphig_free( &deferred,
                    NULL );
    }

    return ret;
}
