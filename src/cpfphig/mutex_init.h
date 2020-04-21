#ifndef CPFPHIG_MUTEX_INIT_H
#define CPFPHIG_MUTEX_INIT_H

#include "cpfphig/cpfphig.h"
#include "cpfphig/mutex.h"
#include "cpfphig/mutex_attr.h"

cpfphig
cpfphig_mutex_init( struct cpfphig_mutex*                    Mutex,
                  struct cpfphig_mutex_attr*               Mutex_Attr,
                  CPFPHIG_OPTIONAL struct cpfphig_error*   Error );

#endif
