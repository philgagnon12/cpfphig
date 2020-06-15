#ifndef CPFPHIG_REALLOC_H
#define CPFPHIG_REALLOC_H

#include "cpfphig/cpfphig.h"

cpfphig
cpfphig_realloc( const size_t                               Size,
                 void*                                      Ptr_Src,
                 void**                                     Ptr_Dest,
                 CPFPHIG_OPTIONAL struct cpfphig_error*     Error );

#endif

