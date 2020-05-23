
#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_UNISTD_H

#include <unistd.h>

cpfphig
cpfphig_sleep( int                                      Milliseconds,
               CPFPHIG_OPTIONAL struct cpfphig_error*   Error )
{
    if( Milliseconds <= 0 )
    {
        return CPFPHIG_OK;
    }

    if( 0 != usleep( Milliseconds * 1000 ) )
    {
        if( Error != NULL )
            cpfphig_error_message( cpfphig_system_error, "usleep failed", Error );

        return CPFPHIG_FAIL;
    }

    return CPFPHIG_OK;
}

#endif
