#include "cpfphig/cpfphig.h"
#include "cpfphig/malloc.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

cpfphig
cpfphig_free( void*                                   Ptr,
            CPFPHIG_OPTIONAL struct cpfphig_error*    Error )
{
    assert_non_null( (void**)Ptr );

    void** ptr = Ptr;

    Ptr = *(void**)Ptr;

    check_expected( Ptr );

    test_free( Ptr );

     *(void**)ptr = NULL;

    return (cpfphig)mock();
}
