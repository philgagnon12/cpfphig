#ifndef MELPHIG_STRNSTR_H
#define MELPHIG_STRNSTR_H

#include "melphig/melphig.h"

#include <stdlib.h>

mphig
mphig_strnstr( const char*                          Haystack,
               const char*                          Needle,
               size_t                               Len,
               char** const                         Occurence,
               MELPHIG_OPTIONAL struct mphig_error* Error );

#endif
