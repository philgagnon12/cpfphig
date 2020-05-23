#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_PTHREAD_H

#include "cpfphig/thread_cond_destroy.h"
#include "cpfphig/thread_cond.h"
#include "cpfphig/free.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

cpfphig
cpfphig_thread_cond_destroy( struct cpfphig_thread_cond*               Cond,
                             CPFPHIG_OPTIONAL struct cpfphig_error*    Error )
{
    int     destroy_ret = 0;

    // NULL checks
    if( Cond == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message( cpfphig_system_error, "Cond is NULL", Error );

        return CPFPHIG_FAIL;
    }

    if( 0 != ( destroy_ret = pthread_cond_destroy( Cond->pthread_cond ) ) )
    {
        if( Error != NULL )
        {
            cpfphig_error_message( cpfphig_system_error,
                                   "pthread_cond_destroy failed with error %d, EINVAL(%d) EBUSY(%d)",
                                   Error,
                                   destroy_ret,
                                   EINVAL,
                                   EBUSY );
        }

        return CPFPHIG_FAIL;
    }

    return cpfphig_free( &Cond->pthread_cond,
                         NULL );
}

#endif
