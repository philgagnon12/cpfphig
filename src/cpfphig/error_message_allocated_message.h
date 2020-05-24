#ifndef CPFPHIG_ERROR_MESSAGE_ALLOCATED_MESSAGE_H
#define CPFPHIG_ERROR_MESSAGE_ALLOCATED_MESSAGE_H

#include "cpfphig/cpfphig.h"

#include <stdarg.h>

cpfphig
cpfphig_error_message_allocated_message( struct cpfphig_error*            Error,
                                         const char*                      Format,
                                         va_list                          Args );

#endif
