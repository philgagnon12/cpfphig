#include "cpfphig/cpfphig.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

cpfphig
cpfphig_error_message_call( enum cpfphig_error_type          Error_Type,
                            const char*                      Format,
                            CPFPHIG_OPTIONAL const char*     File,
                            CPFPHIG_OPTIONAL const char*     Function,
                            CPFPHIG_OPTIONAL int             Line,
                            struct cpfphig_error*            Error,
                            ... )
{
    assert_non_null( Error );
    assert_non_null( Format );

    check_expected( Error_Type );
    check_expected( Format );

    Error->error_type   = Error_Type;

    return CPFPHIG_OK;
}
