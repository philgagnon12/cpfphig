#include "melphig/melphig.h"
#include "melphig/modules_load_all.h"
#include "melphig/list.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

fphig
fphig_modules_load_all( struct fphig_list                         Modules_Directories,
                        fphig_module_load_symbol*                 Mphig_Module_Load,
                        struct fphig_list*                        Modules,
                        FPHIG_OPTIONAL struct fphig_error*      Error )
{
    struct fphig_list*      modules = NULL;
    fphig                   ret     = FPHIG_FAIL;

    assert_non_null( Mphig_Module_Load );
    assert_non_null( Modules );

    ret = (fphig)mock();
    if( ret == FPHIG_OK )
    {
        modules = (struct fphig_list*)mock();

        *Modules = *modules;
    }

    return ret;
}
