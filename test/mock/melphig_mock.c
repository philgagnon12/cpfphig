#include "fphig/fphig.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

fphig
fphig_error_message( enum fphig_error_type          Error_Type,
                     const char*                    Message,
                     struct fphig_error*            Error,
                     FPHIG_OPTIONAL const char*   File,
                     FPHIG_OPTIONAL const char*   Function,
                     FPHIG_OPTIONAL size_t        Line )
{
    assert_non_null( Error );
    assert_non_null( Message );

    check_expected( Error_Type );
    check_expected( Message );

    Error->error_type   = Error_Type;
    Error->message_size = FPHIG_ERROR_MESSAGE_SIZE;

    return FPHIG_OK;
}
