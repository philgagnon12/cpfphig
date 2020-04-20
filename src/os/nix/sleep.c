
#include "melphig/melphig.h"

#ifdef FPHIG_HAVE_UNISTD_H

#include <unistd.h>

fphig
fphig_sleep( int Milliseconds,
             FPHIG_OPTIONAL struct fphig_error*   Error )
{
    if( Milliseconds <= 0 )
    {
        return FPHIG_OK;
    }

    if( 0 != usleep( Milliseconds * 1000 ) )
    {
        if( Error != NULL )
            fphig_error_message( fphig_system_error, "usleep failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    return FPHIG_OK;
}

#endif
