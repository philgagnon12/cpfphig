#include "melphig/melphig.h"

#ifdef MELPHIG_HAVE_PTHREAD_H

#include "melphig/malloc.h"
#include "melphig/mutex_init.h"
#include "melphig/mutex.h"
#include "melphig/mutex_attr.h"
#include "melphig/mutex_destroy.h"
#include "melphig/free.h"

#include <pthread.h>

mphig
mphig_mutex_init( struct mphig_mutex*                    Mutex,
                  struct mphig_mutex_attr*               Mutex_Attr,
                  MELPHIG_OPTIONAL struct mphig_error*   Error )
{
    mphig   ret         = MELPHIG_FAIL;
    mphig   init_ret    = MELPHIG_FAIL;

    // NULL checks
    if( Mutex == NULL || Mutex_Attr == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Mutex or Mutex_Attr is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    if( 0 != pthread_mutexattr_init( &(Mutex_Attr->pthread_mutex_attr) ) )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "pthread_mutexattr_init failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    if( MELPHIG_OK == ( ret = mphig_malloc( sizeof( pthread_mutex_t ),
                                            &Mutex->pthread_mutex,
                                            Error ) ) )
    {

        if( 0 != pthread_mutex_init( Mutex->pthread_mutex,
                                     &(Mutex_Attr->pthread_mutex_attr) ) )
        {
            if( Error != NULL )
                mphig_error_message(mphig_system_error, "pthread_mutex_init failed", Error, __FILE__, __FUNCTION__, __LINE__ );

            ret = MELPHIG_FAIL;
        }
        if( ret == MELPHIG_OK )
        {
            init_ret = MELPHIG_OK;
        }
    }

    if( 0 != pthread_mutexattr_destroy( &(Mutex_Attr->pthread_mutex_attr) ) )
    {
        if( ret == MELPHIG_OK )
        {
            if( Error != NULL )
                mphig_error_message(mphig_system_error, "pthread_mutexattr_destroy failed", Error, __FILE__, __FUNCTION__, __LINE__ );

            ret = MELPHIG_FAIL;
        }
    }

    if( ret == MELPHIG_FAIL )
    {
        if( init_ret == MELPHIG_OK )
        {
            mphig_mutex_destroy( Mutex,
                                 NULL );
        }

        mphig_free( &Mutex->pthread_mutex,
                    NULL );
    }

    return ret;
}

#endif
