#ifndef MELPHIG_FREE_H
#define MELPHIG_FREE_H

#include "melphig/melphig.h"

// IMPORTANT, Argument Ptr is declared as void*, but is handle as void** internally
// Declared as void* to avoid compiler warnings
// HOW TO USE
// int* number = NULL;
// mphig_malloc( sizeof( int ), &number, NULL );
// mphig_free( &number, NULL );
mphig
mphig_free( void*                                   Ptr,
            MELPHIG_OPTIONAL struct mphig_error*    Error );

#endif
