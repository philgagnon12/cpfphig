#include "melphig/melphig.h"
#include "melphig/modules_load_all.h"
#include "melphig/list.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

mphig
mphig_modules_load_all( struct mphig_list                         Modules_Directories,
                        mphig_module_load_symbol*                 Mphig_Module_Load,
                        struct mphig_list*                        Modules,
                        MELPHIG_OPTIONAL struct mphig_error*      Error )
{
    struct mphig_list*      modules = NULL;
    mphig                   ret     = MELPHIG_FAIL;

    assert_non_null( Mphig_Module_Load );
    assert_non_null( Modules );

    ret = (mphig)mock();
    if( ret == MELPHIG_OK )
    {
        modules = (struct mphig_list*)mock();

        *Modules = *modules;
    }

    return ret;
}
