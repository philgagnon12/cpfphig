#ifndef FPHIG_LIBRARY_SYM_H
#define FPHIG_LIBRARY_SYM_H

#include "fphig/fphig.h"

fphig
fphig_library_sym( void*                                    Handle,
                   char*                                    Sym_Name,
                   void**                                   Sym,
                   FPHIG_OPTIONAL struct fphig_error*     Error );

#endif
