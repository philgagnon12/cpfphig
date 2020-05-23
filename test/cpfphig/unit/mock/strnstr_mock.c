#include "cpfphig/cpfphig.h"
#include "cpfphig/strnstr.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

cpfphig
cpfphig_strnstr( const char*                            Haystack,
                 const char*                            Needle,
                 size_t                                 Len,
                 char** const                           Occurence,
                 CPFPHIG_OPTIONAL struct cpfphig_error* Error )
{
    cpfphig ret         = CPFPHIG_FAIL;
    char*   occurence   = NULL;

    assert_non_null( Haystack );
    assert_non_null( Needle );
    assert_non_null( Occurence );

    check_expected( Haystack );
    check_expected( Needle );
    check_expected( Len );

    ret = (cpfphig)mock();
    if( CPFPHIG_OK == ret )
    {
        occurence = (char*)mock_ptr_type(char*);
        *Occurence = occurence;
    }

    return ret;
}


