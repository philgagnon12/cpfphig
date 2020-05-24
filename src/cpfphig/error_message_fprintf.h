#ifndef CPFPHIG_ERROR_MESSAGE_FPRINTF_H
#define CPFPHIG_ERROR_MESSAGE_FPRINTF_H

#include "cpfphig/cpfphig.h"

#include <stdarg.h>

cpfphig
cpfphig_error_message_fprintf( struct cpfphig_error*            Error,
                               const char*                      Format,
                               va_list                          Args );

#endif

