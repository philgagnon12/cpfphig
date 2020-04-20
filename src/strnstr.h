#ifndef FPHIG_STRNSTR_H
#define FPHIG_STRNSTR_H

#include "melphig/melphig.h"

#include <stdlib.h>

fphig
fphig_strnstr( const char*                          Haystack,
               const char*                          Needle,
               size_t                               Len,
               char** const                         Occurence,
               FPHIG_OPTIONAL struct fphig_error* Error );

#endif
