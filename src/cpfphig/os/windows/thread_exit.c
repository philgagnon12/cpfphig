#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_WINDOWS_H

#include "cpfphig/thread_exit.h"

#include <windows.h>

void
cpfphig_thread_exit( int Ret )
{
    ExitThread( Ret );
}

#endif
