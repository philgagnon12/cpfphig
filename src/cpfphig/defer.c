#include "cpfphig/cpfphig.h"
#include "cpfphig/defer.h"
#include "cpfphig/thread_pool.h"
#include "cpfphig/malloc.h"
#include "cpfphig/thread_pool_task.h"
#include "cpfphig/free.h"
#include "cpfphig/sleep.h"
#include "cpfphig/assert.h"

static
void
defer_routine( void* Arg )
{
    struct cpfphig_deferred* deferred = NULL;

    cpfphig_assert( NULL != Arg,
                  "Arg is NULL",
                  __FILE__,
                  __FUNCTION__,
                  __LINE__ );

    deferred = Arg;

    cpfphig_assert( CPFPHIG_OK == cpfphig_sleep( deferred->delay_milliseconds, NULL ),
                  "cpfphig_sleep failed",
                  __FILE__,
                  __FUNCTION__,
                  __LINE__ );

    deferred->routine( deferred->routine_arg );
}

cpfphig
cpfphig_defer( cpfphig_defer_routine_symbol*            Routine,
               void*                                    Routine_Arg,
               int                                      Delay_Milliseconds,
               struct cpfphig_thread_pool*              Thread_Pool,
               CPFPHIG_OPTIONAL struct cpfphig_error*   Error )
{
    cpfphig                               ret             = CPFPHIG_FAIL;
    struct cpfphig_deferred*              deferred        = NULL;
    static const struct cpfphig_deferred  const_deferred  = CPFPHIG_CONST_CPFPHIG_DEFERRED;

    // NULL checks
    if( Routine == NULL || Thread_Pool == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Routine or Thread_Pool is NULL", Error );

        return CPFPHIG_FAIL;
    }

    if( CPFPHIG_FAIL == cpfphig_malloc( sizeof( struct cpfphig_deferred ),
                                        &deferred,
                                        Error ) )
    {
        return CPFPHIG_FAIL;
    }

    *deferred = const_deferred;

    deferred->routine               = Routine;
    deferred->routine_arg           = Routine_Arg;
    deferred->delay_milliseconds    = Delay_Milliseconds;

    if( CPFPHIG_OK == ( ret = cpfphig_thread_pool_task( Thread_Pool,
                                                        &defer_routine,
                                                        deferred,
                                                        Error ) ) )
    {
        ret = CPFPHIG_OK;
    }

    if( ret == CPFPHIG_FAIL )
    {
        cpfphig_free( &deferred,
                      NULL );
    }

    return ret;
}
