#ifndef MELPHIG_ASSERT_H
#define MELPHIG_ASSERT_H

#include <assert.h>
#include <stdio.h>

#define mphig_assert( Condition, Message, File, Function, Line ) if(!(Condition)){ mphig_assert_failed( Message, File, Function, Line); assert(Condition); }

void
mphig_assert_failed( const char*    Message,
                     const char*    File,
                     const char*    Function,
                     size_t         Line );


#endif
