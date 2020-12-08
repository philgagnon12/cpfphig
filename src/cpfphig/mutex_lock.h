#ifndef CPFPHIG_MUTEX_LOCK_H
#define CPFPHIG_MUTEX_LOCK_H

#include "cpfphig/cpfphig.h"
#include "cpfphig/mutex.h"

#ifdef __cplusplus
extern "C" 
{
#endif

cpfphig
cpfphig_mutex_lock( struct cpfphig_mutex*                    Mutex,
                    CPFPHIG_OPTIONAL struct cpfphig_error*   Error );

#ifdef __cplusplus
}
#endif

#endif
