#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_DLFCN_H

#include "cpfphig/cpfphig.h"
#include "cpfphig/library_sym.h"

#include <dlfcn.h>

cpfphig
cpfphig_library_sym( void*                                  Handle,
                     char*                                  Sym_Name,
                     void**                                 Sym,
                     CPFPHIG_OPTIONAL struct cpfphig_error* Error )
{
    void*   sym = NULL;

    // NULL checks
    if( Handle == NULL || Sym_Name == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Handle or Sym_Name is NULL", Error );

        return CPFPHIG_FAIL;
    }

    if( NULL == ( sym = dlsym( Handle, Sym_Name ) ) )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "dlsym failed", Error );

        return CPFPHIG_FAIL;
    }

    *Sym = sym;

    return CPFPHIG_OK;
}

#endif

