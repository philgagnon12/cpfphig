#ifndef CPFPHIG_MALLOC_H
#define CPFPHIG_MALLOC_H

// IMPORTANT, Argument Ptr is declared as void*, but is handle as void** internally
// Declared as void* to avoid compiler warnings
// HOW TO USE
// int* number = NULL;
// cpfphig_malloc( sizeof( int ), &number, NULL );
// cpfphig_free( &number, NULL );
cpfphig
cpfphig_malloc( const size_t                          Size,
              void*                                 Ptr,
              CPFPHIG_OPTIONAL struct cpfphig_error*  Error );

#endif
