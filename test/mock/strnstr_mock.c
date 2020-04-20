#include "fphig/fphig.h"
#include "fphig/strnstr.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

fphig
fphig_strnstr( const char*                          Haystack,
               const char*                          Needle,
               size_t                               Len,
               char** const                         Occurence,
               FPHIG_OPTIONAL struct fphig_error* Error )
{
    fphig   ret         = FPHIG_FAIL;
    char*   occurence   = NULL;

    assert_non_null( Haystack );
    assert_non_null( Needle );
    assert_non_null( Occurence );

    check_expected( Haystack );
    check_expected( Needle );
    check_expected( Len );

    ret = (fphig)mock();
    if( FPHIG_OK == ret )
    {
        occurence = (char*)mock_ptr_type(char*);
        *Occurence = occurence;
    }

    return ret;
}


