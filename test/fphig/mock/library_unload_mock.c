#include "fphig/fphig.h"
#include "fphig/library_unload.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

fphig
fphig_library_unload( void*                                    Handle,
                      FPHIG_OPTIONAL struct fphig_error*     Error )
{
    fphig                   ret    = FPHIG_FAIL;

    assert_non_null( Handle );

    ret = (fphig)mock();

    return ret;
}
