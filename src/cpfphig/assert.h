#ifndef CPFPHIG_ASSERT_H
#define CPFPHIG_ASSERT_H

#include <assert.h>
#include <stdio.h>

#define cpfphig_assert( Condition, Message, File, Function, Line ) if(!(Condition)){ cpfphig_assert_failed( Message, File, Function, Line); assert(Condition); }

void
cpfphig_assert_failed( const char*    Message,
                     const char*    File,
                     const char*    Function,
                     size_t         Line );


#endif
