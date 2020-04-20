#include "melphig/melphig.h"
#include "melphig/malloc.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

mphig
mphig_malloc( const size_t                          Size,
              void*                                 Ptr,
              MELPHIG_OPTIONAL struct mphig_error*  Error )
{
    mphig ret;

    assert_non_null( (void**)Ptr );

    check_expected( Size );

    ret = (mphig)mock();

    if( ret == MELPHIG_OK )
    {
        *(void**)Ptr = test_malloc( Size );
    }

    return ret;
}


