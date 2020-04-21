#ifndef CPFPHIG_STRNSTR_H
#define CPFPHIG_STRNSTR_H

#include "cpfphig/cpfphig.h"

#include <stdlib.h>

cpfphig
cpfphig_strnstr( const char*                          Haystack,
               const char*                          Needle,
               size_t                               Len,
               char** const                         Occurence,
               CPFPHIG_OPTIONAL struct cpfphig_error* Error );

#endif
