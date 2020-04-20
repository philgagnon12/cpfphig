
#include "melphig/melphig.h"

#ifdef MELPHIG_HAVE_UNISTD_H

#include <unistd.h>

mphig
mphig_sleep( int Milliseconds,
             MELPHIG_OPTIONAL struct mphig_error*   Error )
{
    if( Milliseconds <= 0 )
    {
        return MELPHIG_OK;
    }

    if( 0 != usleep( Milliseconds * 1000 ) )
    {
        if( Error != NULL )
            mphig_error_message( mphig_system_error, "usleep failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    return MELPHIG_OK;
}

#endif
