#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_PTHREAD_H

#include "cpfphig/thread_cond_init.h"
#include "cpfphig/malloc.h"
#include "cpfphig/thread_cond_attr_init.h"
#include "cpfphig/thread_cond_attr_destroy.h"
#include "cpfphig/thread_cond.h"
#include "cpfphig/thread_cond_attr.h"
#include "cpfphig/thread_cond_destroy.h"
#include "cpfphig/free.h"

#include <stdio.h>

cpfphig
cpfphig_thread_cond_init( struct cpfphig_thread_cond*               Cond,
                          struct cpfphig_thread_cond_attr*          Cond_Attr,
                          CPFPHIG_OPTIONAL struct cpfphig_error*    Error )
{
    cpfphig               ret                   = CPFPHIG_FAIL;
    cpfphig               init_ret              = CPFPHIG_FAIL;
    struct cpfphig_error  cond_destroy_error    = CPFPHIG_CONST_CPFPHIG_ERROR;

    // NULL checks
    if( Cond == NULL || Cond_Attr == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message( cpfphig_system_error, "Cond or Cond_Attr is NULL", Error );

        return CPFPHIG_FAIL;
    }

    if( CPFPHIG_FAIL == cpfphig_thread_cond_attr_init( Cond_Attr,
                                                       Error ) )
    {
        return CPFPHIG_FAIL;
    }

    if( CPFPHIG_OK == ( ret = cpfphig_malloc( sizeof( pthread_cond_t ),
                                              &Cond->pthread_cond,
                                              Error ) ) )
    {
        if( 0 != pthread_cond_init( Cond->pthread_cond,
                                    &(Cond_Attr->pthread_cond_attr ) ) )
        {
            if( Error != NULL )
                cpfphig_error_message( cpfphig_system_error, "pthread_cond_init failed", Error );

            ret = CPFPHIG_FAIL;
        }
        if( ret == CPFPHIG_OK )
        {
            init_ret = CPFPHIG_OK;
        }
    }

    if( CPFPHIG_FAIL == cpfphig_thread_cond_attr_destroy( Cond_Attr,
                                                          &cond_destroy_error ) )
    {
        if( ret == CPFPHIG_OK )
        {
            if( Error != NULL )
                *Error = cond_destroy_error;

            ret = CPFPHIG_FAIL;
        }
    }

    if( ret == CPFPHIG_FAIL )
    {
        if( init_ret == CPFPHIG_OK )
        {
            cpfphig_thread_cond_destroy( Cond,
                                         NULL );
        }

        cpfphig_free( &Cond->pthread_cond,
                      NULL );
    }

    return ret;
}

#endif
