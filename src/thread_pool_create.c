#include "melphig/melphig.h"
#include "melphig/thread_pool.h"
#include "melphig/mutex_attr.h"
#include "melphig/mutex_init.h"

mphig
mphig_thread_pool_create( struct mphig_thread_pool*             Thread_Pool,
                          MELPHIG_OPTIONAL struct mphig_error*  Error )
{
    mphig                       ret             = MELPHIG_FAIL;
    struct mphig_mutex_attr     mutex_attr      = MELPHIG_CONST_MPHIG_MUTEX_ATTR;

    // NULL checks
    if( Thread_Pool == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Thread_Pool is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    return mphig_mutex_init( &Thread_Pool->mutex,
                             &mutex_attr,
                             Error );
}
