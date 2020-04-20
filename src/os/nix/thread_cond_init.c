#include "melphig/melphig.h"

#ifdef FPHIG_HAVE_PTHREAD_H

#include "melphig/thread_cond_init.h"
#include "melphig/malloc.h"
#include "melphig/thread_cond_attr_init.h"
#include "melphig/thread_cond_attr_destroy.h"
#include "melphig/thread_cond.h"
#include "melphig/thread_cond_attr.h"
#include "melphig/thread_cond_destroy.h"
#include "melphig/free.h"

#include <stdio.h>

fphig
fphig_thread_cond_init( struct fphig_thread_cond*               Cond,
                        struct fphig_thread_cond_attr*          Cond_Attr,
                        FPHIG_OPTIONAL struct fphig_error*    Error )
{
    fphig               ret                             =   FPHIG_FAIL;
    fphig               init_ret                        =   FPHIG_FAIL;
    struct fphig_error  cond_destroy_error              = FPHIG_CONST_MPHIG_ERROR;

    // NULL checks
    if( Cond == NULL || Cond_Attr == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "Cond or Cond_Attr is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    if( FPHIG_FAIL == fphig_thread_cond_attr_init( Cond_Attr,
                                                     Error ) )
    {
        return FPHIG_FAIL;
    }

    if( FPHIG_OK == ( ret = fphig_malloc( sizeof( pthread_cond_t ),
                                            &Cond->pthread_cond,
                                            Error ) ) )
    {
        if( 0 != pthread_cond_init( Cond->pthread_cond,
                                    &(Cond_Attr->pthread_cond_attr ) ) )
        {
            if( Error != NULL )
                fphig_error_message(fphig_system_error, "pthread_cond_init failed", Error, __FILE__, __FUNCTION__, __LINE__ );

            ret = FPHIG_FAIL;
        }
        if( ret == FPHIG_OK )
        {
            init_ret = FPHIG_OK;
        }
    }

    if( FPHIG_FAIL == fphig_thread_cond_attr_destroy( Cond_Attr,
                                                        &cond_destroy_error ) )
    {
        if( ret == FPHIG_OK )
        {
            if( Error != NULL )
                *Error = cond_destroy_error;

            ret = FPHIG_FAIL;
        }
    }

    if( ret == FPHIG_FAIL )
    {
        if( init_ret == FPHIG_OK )
        {
            fphig_thread_cond_destroy( Cond,
                                       NULL );
        }

        fphig_free( &Cond->pthread_cond,
                    NULL );
    }

    return ret;
}

#endif
