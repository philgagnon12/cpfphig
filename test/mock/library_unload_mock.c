#include "melphig/melphig.h"
#include "melphig/library_unload.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

mphig
mphig_library_unload( void*                                    Handle,
                      MELPHIG_OPTIONAL struct mphig_error*     Error )
{
    mphig                   ret    = MELPHIG_FAIL;

    assert_non_null( Handle );

    ret = (mphig)mock();

    return ret;
}
