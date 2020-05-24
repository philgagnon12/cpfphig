#include "cpfphig/cpfphig.h"
#include "cpfphig/error_message_allocated_message.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

cpfphig
cpfphig_error_message_allocated_message( struct cpfphig_error*            Error,
                                         const char*                      Format,
                                         va_list                          Args )
{
    assert_non_null( Error );
    assert_non_null( Format );

    check_expected( Format );

    return CPFPHIG_OK;
}
