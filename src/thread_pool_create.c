#include "melphig/melphig.h"
#include "melphig/thread_pool.h"
#include "melphig/mutex_attr.h"
#include "melphig/mutex_init.h"

fphig
fphig_thread_pool_create( struct fphig_thread_pool*             Thread_Pool,
                          FPHIG_OPTIONAL struct fphig_error*  Error )
{
    fphig                       ret             = FPHIG_FAIL;
    struct fphig_mutex_attr     mutex_attr      = FPHIG_CONST_MPHIG_MUTEX_ATTR;

    // NULL checks
    if( Thread_Pool == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "Thread_Pool is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    return fphig_mutex_init( &Thread_Pool->mutex,
                             &mutex_attr,
                             Error );
}
