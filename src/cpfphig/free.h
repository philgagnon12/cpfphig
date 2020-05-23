#ifndef CPFPHIG_FREE_H
#define CPFPHIG_FREE_H

#include "cpfphig/cpfphig.h"

// IMPORTANT, Argument Ptr is declared as void*, but is handle as void** internally
// Declared as void* to avoid compiler warnings
// HOW TO USE
// int* number = NULL;
// cpfphig_malloc( sizeof( int ), &number, NULL );
// cpfphig_free( &number, NULL );
cpfphig
cpfphig_free( void*                                     Ptr,
              CPFPHIG_OPTIONAL struct cpfphig_error*    Error );

#endif
