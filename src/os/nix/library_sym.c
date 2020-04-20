#include "melphig/melphig.h"

#ifdef MELPHIG_HAVE_DLFCN_H

#include "melphig/melphig.h"
#include "melphig/library_sym.h"

#include <dlfcn.h>

mphig
mphig_library_sym( void*                                    Handle,
                   char*                                    Sym_Name,
                   void**                                   Sym,
                   MELPHIG_OPTIONAL struct mphig_error*     Error )
{
    void*   sym = NULL;

    // NULL checks
    if( Handle == NULL || Sym_Name == NULL )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "Handle or Sym_Name is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    if( NULL == ( sym = dlsym( Handle, Sym_Name ) ) )
    {
        if( Error != NULL )
            mphig_error_message(mphig_system_error, "dlsym failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return MELPHIG_FAIL;
    }

    *Sym = sym;

    return MELPHIG_OK;
}

#endif

