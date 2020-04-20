#include "melphig/melphig.h"

#ifdef MELPHIG_HAVE_PTHREAD_H

#include "melphig/thread_cond_attr_destroy.h"
#include "melphig/thread_cond_attr.h"

#include <pthread.h>

mphig
mphig_thread_cond_attr_destroy( struct mphig_thread_cond_attr*          Cond_Attr,
                                MELPHIG_OPTIONAL struct mphig_error*    Error )
{
    // NULL checks
    if( Cond_Attr == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Cond_Attr is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    if( 0 != pthread_condattr_destroy( &(Cond_Attr->pthread_cond_attr) ) )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "pthread_condattr_destroy failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    return MELPHIG_OK;
}

#endif
