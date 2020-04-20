#ifndef FPHIG_ASSERT_H
#define FPHIG_ASSERT_H

#include <assert.h>
#include <stdio.h>

#define fphig_assert( Condition, Message, File, Function, Line ) if(!(Condition)){ fphig_assert_failed( Message, File, Function, Line); assert(Condition); }

void
fphig_assert_failed( const char*    Message,
                     const char*    File,
                     const char*    Function,
                     size_t         Line );


#endif
