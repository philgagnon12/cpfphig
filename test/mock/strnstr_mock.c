#include "melphig/melphig.h"
#include "melphig/strnstr.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

mphig
mphig_strnstr( const char*                          Haystack,
               const char*                          Needle,
               size_t                               Len,
               char** const                         Occurence,
               MELPHIG_OPTIONAL struct mphig_error* Error )
{
    mphig   ret         = MELPHIG_FAIL;
    char*   occurence   = NULL;

    assert_non_null( Haystack );
    assert_non_null( Needle );
    assert_non_null( Occurence );

    check_expected( Haystack );
    check_expected( Needle );
    check_expected( Len );

    ret = (mphig)mock();
    if( MELPHIG_OK == ret )
    {
        occurence = (char*)mock_ptr_type(char*);
        *Occurence = occurence;
    }

    return ret;
}


