#include "melphig/melphig.h"
#include "melphig/malloc.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

mphig
mphig_free( void*                                   Ptr,
            MELPHIG_OPTIONAL struct mphig_error*    Error )
{
    assert_non_null( (void**)Ptr );

    void** ptr = Ptr;

    Ptr = *(void**)Ptr;

    check_expected( Ptr );

    test_free( Ptr );

     *(void**)ptr = NULL;

    return (mphig)mock();
}
