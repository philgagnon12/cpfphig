#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_WINDOWS_H

#include "cpfphig/thread_cond_attr_init.h"
#include "cpfphig/thread_cond_attr.h"

cpfphig
cpfphig_thread_cond_attr_init( struct cpfphig_thread_cond_attr*          Cond_Attr,
                             CPFPHIG_OPTIONAL struct cpfphig_error*    Error )
{
    // NULL checks
    if( Cond_Attr == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Cond_Attr is NULL", Error );

        return CPFPHIG_FAIL;
    }

    return CPFPHIG_OK;
}

#endif
