#ifndef CPFPHIG_H
#define CPFPHIG_H

#include "cpfphig_config.h"
#include <stdlib.h>

typedef int cpfphig;

#define CPFPHIG_OK      ( 0x00 )
#define CPFPHIG_FAIL    ( 0x01 )

// Used to indicate that an argument is optional
#define CPFPHIG_OPTIONAL

#ifdef CPFPHIG_UNIT_TESTING
#define CPFPHIG_REAL( FunctionName ) real_ ## FunctionName
#else
#define CPFPHIG_REAL( FunctionName ) FunctionName
#endif

enum cpfphig_error_type
{
    cpfphig_ok              = 0x01,
    cpfphig_system_error    = 0x02,
    cpfphig_user_error      = 0x03
};

struct cpfphig_error
{
    enum cpfphig_error_type     error_type;
    long                        stderr_pos;
    int                         log_len;
};

#define CPFPHIG_CONST_CPFPHIG_ERROR { \
    cpfphig_system_error,   \
    0,                      \
    0,                      \
}

cpfphig
cpfphig_stderr_printf( enum cpfphig_error_type          Error_Type,
                       const char*                      Format,
                       CPFPHIG_OPTIONAL const char*     File,
                       CPFPHIG_OPTIONAL const char*     Function,
                       CPFPHIG_OPTIONAL int             Line,
                       struct cpfphig_error*            Error,
                       ... );

// Hide Error arg , to not have empty argument for '...' / __VAR_ARGS__
#define cpfphig_error_message( Error_Type, Format, ... ) cpfphig_stderr_printf( Error_Type, Format, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__ )

#endif
