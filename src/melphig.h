#ifndef MELPHIG_H
#define MELPHIG_H

#include "melphig_config.h"
#include <stdlib.h>

typedef int mphig;

#define MELPHIG_OK      ( 0x00 )
#define MELPHIG_FAIL    ( 0x01 )

// Used to indicate that an argument is optional
#define MELPHIG_OPTIONAL

#ifdef MELPHIG_UNIT_TESTING
#define REAL( FunctionName ) real_ ## FunctionName
#else
#define REAL( FunctionName ) FunctionName
#endif

enum mphig_error_type{ mphig_ok, mphig_system_error, mphig_user_error };

#define MELPHIG_ERROR_MESSAGE_SIZE      ( 0x0F00 )
#define MELPHIG_ERROR_FILE_SIZE         ( 0x0F00 )
#define MELPHIG_ERROR_FUNCTION_SIZE     ( 0x0F00 )

struct mphig_error
{
    enum mphig_error_type   error_type;
    char                    message[MELPHIG_ERROR_MESSAGE_SIZE];
    size_t                  message_size;

    char                    file[MELPHIG_ERROR_FILE_SIZE];
    size_t                  file_size;

    char                    function[MELPHIG_ERROR_FUNCTION_SIZE];
    size_t                  function_size;

    size_t                  line;
};

#define MELPHIG_CONST_MPHIG_ERROR { \
    mphig_system_error, \
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

mphig
mphig_error_message( enum mphig_error_type          Error_Type,
                     const char*                    Message,
                     struct mphig_error*            Error,
                     MELPHIG_OPTIONAL const char*   File,
                     MELPHIG_OPTIONAL const char*   Function,
                     MELPHIG_OPTIONAL size_t        Line );

#endif
