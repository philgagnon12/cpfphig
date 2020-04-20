#include "melphig/melphig.h"

#ifdef MELPHIG_HAVE_PTHREAD_H

#include "melphig/thread_cond_destroy.h"
#include "melphig/thread_cond.h"
#include "melphig/free.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

#define MELPHIG_BUFFER_SIZE ( 0xFF )

mphig
mphig_thread_cond_destroy( struct mphig_thread_cond*               Cond,
                           MELPHIG_OPTIONAL struct mphig_error*    Error )
{
    int     destroy_ret = 0;
    char    error_message_buffer[ MELPHIG_BUFFER_SIZE ];

    // NULL checks
    if( Cond == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Cond is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    if( 0 != ( destroy_ret = pthread_cond_destroy( Cond->pthread_cond ) ) )
    {
        if( Error != NULL )
        {
            memset( error_message_buffer,
                    0x00,
                    MELPHIG_BUFFER_SIZE );

            snprintf( error_message_buffer,
                      MELPHIG_BUFFER_SIZE,
                      "pthread_cond_destroy failed with error %d, EINVAL(%d) EBUSY(%d)",
                      destroy_ret,
                      EINVAL,
                      EBUSY );

            mphig_error_message(mphig_system_error, error_message_buffer, Error, __FILE__, __FUNCTION__, __LINE__ );
        }

        return MELPHIG_FAIL;
    }

    return mphig_free( &Cond->pthread_cond,
                       NULL );
}

#endif
