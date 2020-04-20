#include "fphig/fphig.h"
#include "fphig/malloc.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

fphig
fphig_free( void*                                   Ptr,
            FPHIG_OPTIONAL struct fphig_error*    Error )
{
    assert_non_null( (void**)Ptr );

    void** ptr = Ptr;

    Ptr = *(void**)Ptr;

    check_expected( Ptr );

    test_free( Ptr );

     *(void**)ptr = NULL;

    return (fphig)mock();
}
