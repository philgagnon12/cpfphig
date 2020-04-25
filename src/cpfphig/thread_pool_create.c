#include "cpfphig/cpfphig.h"
#include "cpfphig/thread_pool.h"
#include "cpfphig/mutex_attr.h"
#include "cpfphig/mutex_init.h"

cpfphig
cpfphig_thread_pool_create( struct cpfphig_thread_pool*             Thread_Pool,
                            CPFPHIG_OPTIONAL struct cpfphig_error*  Error )
{
    cpfphig                       ret             = CPFPHIG_FAIL;
    struct cpfphig_mutex_attr     mutex_attr      = CPFPHIG_CONST_CPFPHIG_MUTEX_ATTR;

    // NULL checks
    if( Thread_Pool == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Thread_Pool is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    return cpfphig_mutex_init( &Thread_Pool->mutex,
                               &mutex_attr,
                               Error );
}
