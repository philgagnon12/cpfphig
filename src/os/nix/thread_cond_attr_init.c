#include "melphig/melphig.h"

#ifdef FPHIG_HAVE_PTHREAD_H

#include "melphig/thread_cond_attr_init.h"
#include "melphig/thread_cond_attr.h"

#include <pthread.h>

fphig
fphig_thread_cond_attr_init( struct fphig_thread_cond_attr*          Cond_Attr,
                             FPHIG_OPTIONAL struct fphig_error*    Error )
{
    // NULL checks
    if( Cond_Attr == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "Cond_Attr is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    if( 0 != pthread_condattr_init( &(Cond_Attr->pthread_cond_attr) ) )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "pthread_condattr_init failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    return FPHIG_OK;
}

#endif
