#ifndef CPFPHIG_THREAD_CREATE_H
#define CPFPHIG_THREAD_CREATE_H

#include "cpfphig/cpfphig.h"
#include "cpfphig/thread.h"
#include "cpfphig/thread_attr.h"

cpfphig
cpfphig_thread_create( struct cpfphig_thread*                   Thread,
                       struct cpfphig_thread_attr*              Thread_Attr,
                       int (*Start_Routine)(void*),
                       CPFPHIG_OPTIONAL void*                   Arg,
                       CPFPHIG_OPTIONAL struct cpfphig_error*   Error );
#endif
