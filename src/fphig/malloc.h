#ifndef FPHIG_MALLOC_H
#define FPHIG_MALLOC_H

// IMPORTANT, Argument Ptr is declared as void*, but is handle as void** internally
// Declared as void* to avoid compiler warnings
// HOW TO USE
// int* number = NULL;
// fphig_malloc( sizeof( int ), &number, NULL );
// fphig_free( &number, NULL );
fphig
fphig_malloc( const size_t                          Size,
              void*                                 Ptr,
              FPHIG_OPTIONAL struct fphig_error*  Error );

#endif
