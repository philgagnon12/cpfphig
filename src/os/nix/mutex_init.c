#include "melphig/melphig.h"

#ifdef FPHIG_HAVE_PTHREAD_H

#include "melphig/malloc.h"
#include "melphig/mutex_init.h"
#include "melphig/mutex.h"
#include "melphig/mutex_attr.h"
#include "melphig/mutex_destroy.h"
#include "melphig/free.h"

#include <pthread.h>

fphig
fphig_mutex_init( struct fphig_mutex*                    Mutex,
                  struct fphig_mutex_attr*               Mutex_Attr,
                  FPHIG_OPTIONAL struct fphig_error*   Error )
{
    fphig   ret         = FPHIG_FAIL;
    fphig   init_ret    = FPHIG_FAIL;

    // NULL checks
    if( Mutex == NULL || Mutex_Attr == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "Mutex or Mutex_Attr is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    if( 0 != pthread_mutexattr_init( &(Mutex_Attr->pthread_mutex_attr) ) )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "pthread_mutexattr_init failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    if( FPHIG_OK == ( ret = fphig_malloc( sizeof( pthread_mutex_t ),
                                            &Mutex->pthread_mutex,
                                            Error ) ) )
    {

        if( 0 != pthread_mutex_init( Mutex->pthread_mutex,
                                     &(Mutex_Attr->pthread_mutex_attr) ) )
        {
            if( Error != NULL )
                fphig_error_message(fphig_system_error, "pthread_mutex_init failed", Error, __FILE__, __FUNCTION__, __LINE__ );

            ret = FPHIG_FAIL;
        }
        if( ret == FPHIG_OK )
        {
            init_ret = FPHIG_OK;
        }
    }

    if( 0 != pthread_mutexattr_destroy( &(Mutex_Attr->pthread_mutex_attr) ) )
    {
        if( ret == FPHIG_OK )
        {
            if( Error != NULL )
                fphig_error_message(fphig_system_error, "pthread_mutexattr_destroy failed", Error, __FILE__, __FUNCTION__, __LINE__ );

            ret = FPHIG_FAIL;
        }
    }

    if( ret == FPHIG_FAIL )
    {
        if( init_ret == FPHIG_OK )
        {
            fphig_mutex_destroy( Mutex,
                                 NULL );
        }

        fphig_free( &Mutex->pthread_mutex,
                    NULL );
    }

    return ret;
}

#endif
