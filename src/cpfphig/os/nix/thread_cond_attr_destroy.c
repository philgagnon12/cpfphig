#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_PTHREAD_H

#include "cpfphig/thread_cond_attr_destroy.h"
#include "cpfphig/thread_cond_attr.h"

#include <pthread.h>

cpfphig
cpfphig_thread_cond_attr_destroy( struct cpfphig_thread_cond_attr*          Cond_Attr,
                                CPFPHIG_OPTIONAL struct cpfphig_error*    Error )
{
    // NULL checks
    if( Cond_Attr == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Cond_Attr is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    if( 0 != pthread_condattr_destroy( &(Cond_Attr->pthread_cond_attr) ) )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "pthread_condattr_destroy failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    return CPFPHIG_OK;
}

#endif
