#include "cpfphig/cpfphig.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

cpfphig
cpfphig_error_message( enum cpfphig_error_type          Error_Type,
                     const char*                    Message,
                     struct cpfphig_error*            Error,
                     CPFPHIG_OPTIONAL const char*   File,
                     CPFPHIG_OPTIONAL const char*   Function,
                     CPFPHIG_OPTIONAL size_t        Line )
{
    assert_non_null( Error );
    assert_non_null( Message );

    check_expected( Error_Type );
    check_expected( Message );

    Error->error_type   = Error_Type;
    Error->message_size = CPFPHIG_ERROR_MESSAGE_SIZE;

    return CPFPHIG_OK;
}
