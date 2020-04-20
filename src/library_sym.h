#ifndef MELPHIG_LIBRARY_SYM_H
#define MELPHIG_LIBRARY_SYM_H

#include "melphig/melphig.h"

mphig
mphig_library_sym( void*                                    Handle,
                   char*                                    Sym_Name,
                   void**                                   Sym,
                   MELPHIG_OPTIONAL struct mphig_error*     Error );

#endif
