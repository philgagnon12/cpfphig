#ifndef CPFPHIG_H
#define CPFPHIG_H

#include "cpfphig_config.h"
#include <stdlib.h>
#include <stdio.h>

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

struct cpfphig_error_fprintf
{
    FILE*   file;
    long    file_pos;
    int     log_len;
};

#define CPFPHIG_CONST_CPFPHIG_ERROR_FPRINTF { \
    stderr, \
    0,      \
    0       \
}

struct cpfphig_error_allocated_message
{
    char*  message;
    int    message_len;
};

#define CPFPHIG_CONST_CPFPHIG_ERROR_ALLOCATED_MESSAGE { \
    NULL,   \
    0,      \
}

enum cpfphig_error_component_type
{
    cpfphig_error_fprintf           = 0x01,
    cpfphig_error_allocated_message = 0x02
};

union cpfphig_error_component
{
    struct cpfphig_error_fprintf            fprintf;
    struct cpfphig_error_allocated_message  allocated_message;
};

#define CPFPHIG_CONST_CPFPHIG_ERROR_COMPONENT { \
    CPFPHIG_CONST_CPFPHIG_ERROR_FPRINTF \
}

struct cpfphig_error
{
    enum cpfphig_error_type             error_type;
    const char*                         file;
    const char*                         function;
    int                                 line;
    enum cpfphig_error_component_type   error_component_type;
    union cpfphig_error_component       error_component;

};

#define CPFPHIG_CONST_CPFPHIG_ERROR {       \
    cpfphig_system_error,                   \
    NULL,                                   \
    NULL,                                   \
    0,                                      \
    cpfphig_error_fprintf,                  \
    CPFPHIG_CONST_CPFPHIG_ERROR_COMPONENT   \
}

#define CPFPHIG_CONST_CPFPHIG_FPRINTF_ERROR { \
    cpfphig_system_error,                   \
    NULL,                                   \
    NULL,                                   \
    0,                                      \
    cpfphig_error_fprintf,                  \
    CPFPHIG_CONST_CPFPHIG_ERROR_FPRINTF     \
}

#define CPFPHIG_CONST_CPFPHIG_ALLOCATED_MESSAGE_ERROR { \
    cpfphig_system_error,                           \
    NULL,                                           \
    NULL,                                           \
    0,                                              \
    cpfphig_error_allocated_message,                \
    CPFPHIG_CONST_CPFPHIG_ERROR_ALLOCATED_MESSAGE   \
}

cpfphig
cpfphig_error_message_call( enum cpfphig_error_type          Error_Type,
                            const char*                      Format,
                            CPFPHIG_OPTIONAL const char*     File,
                            CPFPHIG_OPTIONAL const char*     Function,
                            CPFPHIG_OPTIONAL int             Line,
                            struct cpfphig_error*            Error,
                            ... );

// Hide Error arg , to not have empty argument for '...' / __VAR_ARGS__
#define cpfphig_error_message( Error_Type, Format, ... ) cpfphig_error_message_call( Error_Type, Format, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__ )

#endif
