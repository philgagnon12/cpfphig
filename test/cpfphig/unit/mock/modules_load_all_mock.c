#include "cpfphig/cpfphig.h"
#include "cpfphig/modules_load_all.h"
#include "cpfphig/list.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

cpfphig
cpfphig_modules_load_all( struct cpfphig_list                         Modules_Directories,
                        cpfphig_module_load_symbol*                 Mphig_Module_Load,
                        struct cpfphig_list*                        Modules,
                        CPFPHIG_OPTIONAL struct cpfphig_error*      Error )
{
    struct cpfphig_list*      modules = NULL;
    cpfphig                   ret     = CPFPHIG_FAIL;

    assert_non_null( Mphig_Module_Load );
    assert_non_null( Modules );

    ret = (cpfphig)mock();
    if( ret == CPFPHIG_OK )
    {
        modules = (struct cpfphig_list*)mock();

        *Modules = *modules;
    }

    return ret;
}
