#include "fphig/fphig.h"
#include "fphig/malloc.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

fphig
fphig_malloc( const size_t                          Size,
              void*                                 Ptr,
              FPHIG_OPTIONAL struct fphig_error*  Error )
{
    fphig ret;

    assert_non_null( (void**)Ptr );

    check_expected( Size );

    ret = (fphig)mock();

    if( ret == FPHIG_OK )
    {
        *(void**)Ptr = test_malloc( Size );
    }

    return ret;
}


