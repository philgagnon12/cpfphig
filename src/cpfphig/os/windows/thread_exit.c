#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_WINDOWS_H

#include "cpfphig/thread_exit.h"

#include <windows.h>

void
cpfphig_thread_exit( int*                                   Ret,
                     CPFPHIG_OPTIONAL struct cpfphig_error* Error )
{
    int ret = 0;

    if( Ret != NULL )
        ret = *Ret;

    ExitThread( ret );
}

#endif
