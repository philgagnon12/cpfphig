#ifndef FPHIG_FREE_H
#define FPHIG_FREE_H

#include "fphig/fphig.h"

// IMPORTANT, Argument Ptr is declared as void*, but is handle as void** internally
// Declared as void* to avoid compiler warnings
// HOW TO USE
// int* number = NULL;
// fphig_malloc( sizeof( int ), &number, NULL );
// fphig_free( &number, NULL );
fphig
fphig_free( void*                                   Ptr,
            FPHIG_OPTIONAL struct fphig_error*    Error );

#endif
