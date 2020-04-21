#include "cpfphig/cpfphig.h"
#include "cpfphig/library_unload.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

cpfphig
cpfphig_library_unload( void*                                    Handle,
                      CPFPHIG_OPTIONAL struct cpfphig_error*     Error )
{
    cpfphig                   ret    = CPFPHIG_FAIL;

    assert_non_null( Handle );

    ret = (cpfphig)mock();

    return ret;
}
