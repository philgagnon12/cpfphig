#ifndef CPFPHIG_THREAD_COND_INIT_H
#define CPFPHIG_THREAD_COND_INIT_H

#include "cpfphig/cpfphig.h"
#include "cpfphig/thread_cond.h"
#include "cpfphig/thread_cond_attr.h"

cpfphig
cpfphig_thread_cond_init( struct cpfphig_thread_cond*               Cond,
                          struct cpfphig_thread_cond_attr*          Cond_Attr,
                          CPFPHIG_OPTIONAL struct cpfphig_error*    Error );

#endif
