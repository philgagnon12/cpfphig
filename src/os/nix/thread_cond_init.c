#include "melphig/melphig.h"

#ifdef MELPHIG_HAVE_PTHREAD_H

#include "melphig/thread_cond_init.h"
#include "melphig/malloc.h"
#include "melphig/thread_cond_attr_init.h"
#include "melphig/thread_cond_attr_destroy.h"
#include "melphig/thread_cond.h"
#include "melphig/thread_cond_attr.h"
#include "melphig/thread_cond_destroy.h"
#include "melphig/free.h"

#include <stdio.h>

mphig
mphig_thread_cond_init( struct mphig_thread_cond*               Cond,
                        struct mphig_thread_cond_attr*          Cond_Attr,
                        MELPHIG_OPTIONAL struct mphig_error*    Error )
{
    mphig               ret                             =   MELPHIG_FAIL;
    mphig               init_ret                        =   MELPHIG_FAIL;
    struct mphig_error  cond_destroy_error              = MELPHIG_CONST_MPHIG_ERROR;

    // NULL checks
    if( Cond == NULL || Cond_Attr == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Cond or Cond_Attr is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    if( MELPHIG_FAIL == mphig_thread_cond_attr_init( Cond_Attr,
                                                     Error ) )
    {
        return MELPHIG_FAIL;
    }

    if( MELPHIG_OK == ( ret = mphig_malloc( sizeof( pthread_cond_t ),
                                            &Cond->pthread_cond,
                                            Error ) ) )
    {
        if( 0 != pthread_cond_init( Cond->pthread_cond,
                                    &(Cond_Attr->pthread_cond_attr ) ) )
        {
            if( Error != NULL )
                mphig_error_message(mphig_system_error, "pthread_cond_init failed", Error, __FILE__, __FUNCTION__, __LINE__ );

            ret = MELPHIG_FAIL;
        }
        if( ret == MELPHIG_OK )
        {
            init_ret = MELPHIG_OK;
        }
    }

    if( MELPHIG_FAIL == mphig_thread_cond_attr_destroy( Cond_Attr,
                                                        &cond_destroy_error ) )
    {
        if( ret == MELPHIG_OK )
        {
            if( Error != NULL )
                *Error = cond_destroy_error;

            ret = MELPHIG_FAIL;
        }
    }

    if( ret == MELPHIG_FAIL )
    {
        if( init_ret == MELPHIG_OK )
        {
            mphig_thread_cond_destroy( Cond,
                                       NULL );
        }

        mphig_free( &Cond->pthread_cond,
                    NULL );
    }

    return ret;
}

#endif
