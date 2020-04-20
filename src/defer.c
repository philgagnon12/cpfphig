#include "fphig/fphig.h"
#include "fphig/defer.h"
#include "fphig/thread_pool.h"
#include "fphig/malloc.h"
#include "fphig/thread_pool_task.h"
#include "fphig/free.h"
#include "fphig/sleep.h"
#include "fphig/assert.h"

static
void
defer_routine( void* Arg )
{
    struct fphig_deferred* deferred = NULL;

    fphig_assert( NULL != Arg,
                  "Arg is NULL",
                  __FILE__,
                  __FUNCTION__,
                  __LINE__ );

    deferred = Arg;

    fphig_assert( FPHIG_OK == fphig_sleep( deferred->delay_milliseconds, NULL ),
                  "fphig_sleep failed",
                  __FILE__,
                  __FUNCTION__,
                  __LINE__ );

    deferred->routine( deferred->routine_arg );
}

fphig
fphig_defer( fphig_defer_routine_symbol*            Routine,
             void*                                  Routine_Arg,
             int                                    Delay_Milliseconds,
             struct fphig_thread_pool*              Thread_Pool,
             FPHIG_OPTIONAL struct fphig_error*   Error )
{
    fphig                               ret             = FPHIG_FAIL;
    struct fphig_deferred*              deferred        = NULL;
    static const struct fphig_deferred  const_deferred  = FPHIG_CONST_MPHIG_DEFERRED;

    // NULL checks
    if( Routine == NULL || Thread_Pool == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "Routine or Thread_Pool is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    if( FPHIG_FAIL == fphig_malloc( sizeof( struct fphig_deferred ),
                                      &deferred,
                                      Error ) )
    {
        return FPHIG_FAIL;
    }

    *deferred = const_deferred;

    deferred->routine               = Routine;
    deferred->routine_arg           = Routine_Arg;
    deferred->delay_milliseconds    = Delay_Milliseconds;

    if( FPHIG_OK == ( ret = fphig_thread_pool_task( Thread_Pool,
                                                      &defer_routine,
                                                      deferred,
                                                      Error ) ) )
    {
        ret = FPHIG_OK;
    }

    if( ret == FPHIG_FAIL )
    {
        fphig_free( &deferred,
                    NULL );
    }

    return ret;
}
