#ifndef MELPHIG_MALLOC_H
#define MELPHIG_MALLOC_H

// IMPORTANT, Argument Ptr is declared as void*, but is handle as void** internally
// Declared as void* to avoid compiler warnings
// HOW TO USE
// int* number = NULL;
// mphig_malloc( sizeof( int ), &number, NULL );
// mphig_free( &number, NULL );
mphig
mphig_malloc( const size_t                          Size,
              void*                                 Ptr,
              MELPHIG_OPTIONAL struct mphig_error*  Error );

#endif
