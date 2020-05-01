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

enum cpfphig_error_type{ cpfphig_ok, cpfphig_system_error, cpfphig_user_error };

#define CPFPHIG_ERROR_MESSAGE_SIZE      ( 0x0F00 )
#define CPFPHIG_ERROR_FILE_SIZE         ( 0x0F00 )
#define CPFPHIG_ERROR_FUNCTION_SIZE     ( 0x0F00 )

struct cpfphig_error
{
    enum cpfphig_error_type   error_type;
    char                    message[CPFPHIG_ERROR_MESSAGE_SIZE];
    size_t                  message_size;

    char                    file[CPFPHIG_ERROR_FILE_SIZE];
    size_t                  file_size;

    char                    function[CPFPHIG_ERROR_FUNCTION_SIZE];
    size_t                  function_size;

    size_t                  line;
};

#define CPFPHIG_CONST_CPFPHIG_ERROR { \
    cpfphig_system_error, \
    0x00,               \
    0,                  \
    \
    0x00,               \
    0,                  \
    \
    0x00,               \
    0,                  \
    \
    0                   \
}

cpfphig
cpfphig_error_message( enum cpfphig_error_type          Error_Type,
                     const char*                    Message,
                     struct cpfphig_error*            Error,
                     CPFPHIG_OPTIONAL const char*   File,
                     CPFPHIG_OPTIONAL const char*   Function,
                     CPFPHIG_OPTIONAL size_t        Line );

#endif
