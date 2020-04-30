#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_WINDOWS_H

#include "cpfphig/cpfphig.h"
#include "cpfphig/library_sym.h"

#include <windows.h>
#include <Libloaderapi.h>

cpfphig
cpfphig_library_sym( void*                                  Handle,
                     char*                                  Sym_Name,
                     void**                                 Sym,
                     CPFPHIG_OPTIONAL struct cpfphig_error* Error )
{
    FARPROC sym = NULL;

    // NULL checks
    if( Handle == NULL || Sym_Name == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Handle or Sym_Name is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    if( NULL == ( sym = GetProcAddress( (HMODULE)Handle,
                                        Sym_Name ) ) )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "GetProcAddress failed", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    *Sym = (void*)sym;

    return CPFPHIG_OK;
}

#endif

