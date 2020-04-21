#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_PTHREAD_H

#include "cpfphig/thread_cond_destroy.h"
#include "cpfphig/thread_cond.h"
#include "cpfphig/free.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

#define CPFPHIG_BUFFER_SIZE ( 0xFF )

cpfphig
cpfphig_thread_cond_destroy( struct cpfphig_thread_cond*               Cond,
                           CPFPHIG_OPTIONAL struct cpfphig_error*    Error )
{
    int     destroy_ret = 0;
    char    error_message_buffer[ CPFPHIG_BUFFER_SIZE ];

    // NULL checks
    if( Cond == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Cond is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    if( 0 != ( destroy_ret = pthread_cond_destroy( Cond->pthread_cond ) ) )
    {
        if( Error != NULL )
        {
            memset( error_message_buffer,
                    0x00,
                    CPFPHIG_BUFFER_SIZE );

            snprintf( error_message_buffer,
                      CPFPHIG_BUFFER_SIZE,
                      "pthread_cond_destroy failed with error %d, EINVAL(%d) EBUSY(%d)",
                      destroy_ret,
                      EINVAL,
                      EBUSY );

            cpfphig_error_message(cpfphig_system_error, error_message_buffer, Error, __FILE__, __FUNCTION__, __LINE__ );
        }

        return CPFPHIG_FAIL;
    }

    return cpfphig_free( &Cond->pthread_cond,
                       NULL );
}

#endif
