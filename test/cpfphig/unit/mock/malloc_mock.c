#include "cpfphig/cpfphig.h"
#include "cpfphig/malloc.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

cpfphig
cpfphig_malloc( const size_t                            Size,
                void*                                   Ptr,
                CPFPHIG_OPTIONAL struct cpfphig_error*  Error )
{
    cpfphig ret;

    assert_non_null( (void**)Ptr );

    check_expected( Size );

    ret = (cpfphig)mock();

    if( ret == CPFPHIG_OK )
    {
        *(void**)Ptr = test_malloc( Size );
    }

    return ret;
}


