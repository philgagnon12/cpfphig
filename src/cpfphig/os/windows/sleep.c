#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_WINDOWS_H

#include <windows.h>

cpfphig
cpfphig_sleep( int                                      Milliseconds,
               CPFPHIG_OPTIONAL struct cpfphig_error*   Error )
{
    if( Milliseconds <= 0 )
    {
        return CPFPHIG_OK;
    }

    Sleep( Milliseconds ); // return void

    return CPFPHIG_OK;
}

#endif
