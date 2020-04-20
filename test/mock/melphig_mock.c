#include "melphig/melphig.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

mphig
mphig_error_message( enum mphig_error_type          Error_Type,
                     const char*                    Message,
                     struct mphig_error*            Error,
                     MELPHIG_OPTIONAL const char*   File,
                     MELPHIG_OPTIONAL const char*   Function,
                     MELPHIG_OPTIONAL size_t        Line )
{
    assert_non_null( Error );
    assert_non_null( Message );

    check_expected( Error_Type );
    check_expected( Message );

    Error->error_type   = Error_Type;
    Error->message_size = MELPHIG_ERROR_MESSAGE_SIZE;

    return MELPHIG_OK;
}
