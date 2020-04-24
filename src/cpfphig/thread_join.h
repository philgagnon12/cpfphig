#ifndef CPFPHIG_THREAD_JOIN_H
#define CPFPHIG_THREAD_JOIN_H

#include "cpfphig/cpfphig.h"
#include "cpfphig/thread.h"

cpfphig
cpfphig_thread_join( struct cpfphig_thread*                 Thread,
                     CPFPHIG_OPTIONAL int*                  Thread_Ret,
                     CPFPHIG_OPTIONAL struct cpfphig_error* Error );

#endif
