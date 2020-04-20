#include "fphig/fphig.h"

#ifdef FPHIG_HAVE_DLFCN_H

#include "fphig/fphig.h"
#include "fphig/library_sym.h"

#include <dlfcn.h>

fphig
fphig_library_sym( void*                                    Handle,
                   char*                                    Sym_Name,
                   void**                                   Sym,
                   FPHIG_OPTIONAL struct fphig_error*     Error )
{
    void*   sym = NULL;

    // NULL checks
    if( Handle == NULL || Sym_Name == NULL )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "Handle or Sym_Name is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    if( NULL == ( sym = dlsym( Handle, Sym_Name ) ) )
    {
        if( Error != NULL )
            fphig_error_message(fphig_system_error, "dlsym failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return FPHIG_FAIL;
    }

    *Sym = sym;

    return FPHIG_OK;
}

#endif

