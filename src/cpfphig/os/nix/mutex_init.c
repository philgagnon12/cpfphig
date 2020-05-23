#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_PTHREAD_H

#include "cpfphig/malloc.h"
#include "cpfphig/mutex_init.h"
#include "cpfphig/mutex.h"
#include "cpfphig/mutex_attr.h"
#include "cpfphig/mutex_destroy.h"
#include "cpfphig/free.h"

#include <pthread.h>

cpfphig
cpfphig_mutex_init( struct cpfphig_mutex*                    Mutex,
                    struct cpfphig_mutex_attr*               Mutex_Attr,
                    CPFPHIG_OPTIONAL struct cpfphig_error*   Error )
{
    cpfphig   ret         = CPFPHIG_FAIL;
    cpfphig   init_ret    = CPFPHIG_FAIL;

    // NULL checks
    if( Mutex == NULL || Mutex_Attr == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message( cpfphig_system_error, "Mutex or Mutex_Attr is NULL", Error );

        return CPFPHIG_FAIL;
    }

    if( 0 != pthread_mutexattr_init( &(Mutex_Attr->pthread_mutex_attr) ) )
    {
        if( Error != NULL )
            cpfphig_error_message( cpfphig_system_error, "pthread_mutexattr_init failed", Error );

        return CPFPHIG_FAIL;
    }

    if( CPFPHIG_OK == ( ret = cpfphig_malloc( sizeof( pthread_mutex_t ),
                                              &Mutex->pthread_mutex,
                                              Error ) ) )
    {

        if( 0 != pthread_mutex_init( Mutex->pthread_mutex,
                                     &(Mutex_Attr->pthread_mutex_attr) ) )
        {
            if( Error != NULL )
                cpfphig_error_message( cpfphig_system_error, "pthread_mutex_init failed", Error );

            ret = CPFPHIG_FAIL;
        }
        if( ret == CPFPHIG_OK )
        {
            init_ret = CPFPHIG_OK;
        }
    }

    if( 0 != pthread_mutexattr_destroy( &(Mutex_Attr->pthread_mutex_attr) ) )
    {
        if( ret == CPFPHIG_OK )
        {
            if( Error != NULL )
                cpfphig_error_message(cpfphig_system_error, "pthread_mutexattr_destroy failed", Error );

            ret = CPFPHIG_FAIL;
        }
    }

    if( ret == CPFPHIG_FAIL )
    {
        if( init_ret == CPFPHIG_OK )
        {
            cpfphig_mutex_destroy( Mutex,
                                   NULL );
        }

        cpfphig_free( &Mutex->pthread_mutex,
                      NULL );
    }

    return ret;
}

#endif
