#ifndef FPHIG_H
#define FPHIG_H

#include "fphig_config.h"
#include <stdlib.h>

typedef int fphig;

#define FPHIG_OK      ( 0x00 )
#define FPHIG_FAIL    ( 0x01 )

// Used to indicate that an argument is optional
#define FPHIG_OPTIONAL

#ifdef FPHIG_UNIT_TESTING
#define REAL( FunctionName ) real_ ## FunctionName
#else
#define REAL( FunctionName ) FunctionName
#endif

enum fphig_error_type{ fphig_ok, fphig_system_error, fphig_user_error };

#define FPHIG_ERROR_MESSAGE_SIZE      ( 0x0F00 )
#define FPHIG_ERROR_FILE_SIZE         ( 0x0F00 )
#define FPHIG_ERROR_FUNCTION_SIZE     ( 0x0F00 )

struct fphig_error
{
    enum fphig_error_type   error_type;
    char                    message[FPHIG_ERROR_MESSAGE_SIZE];
    size_t                  message_size;

    char                    file[FPHIG_ERROR_FILE_SIZE];
    size_t                  file_size;

    char                    function[FPHIG_ERROR_FUNCTION_SIZE];
    size_t                  function_size;

    size_t                  line;
};

#define FPHIG_CONST_MPHIG_ERROR { \
    fphig_system_error, \
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

fphig
fphig_error_message( enum fphig_error_type          Error_Type,
                     const char*                    Message,
                     struct fphig_error*            Error,
                     FPHIG_OPTIONAL const char*   File,
                     FPHIG_OPTIONAL const char*   Function,
                     FPHIG_OPTIONAL size_t        Line );

#endif
