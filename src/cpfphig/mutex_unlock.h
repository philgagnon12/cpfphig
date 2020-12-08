#ifndef CPFPHIG_MUTEX_UNLOCK_H
#define CPFPHIG_MUTEX_UNLOCK_H

#include "cpfphig/cpfphig.h"
#include "cpfphig/mutex.h"

#ifdef __cplusplus
extern "C" 
{
#endif

cpfphig
cpfphig_mutex_unlock( struct cpfphig_mutex*                    Mutex,
                      CPFPHIG_OPTIONAL struct cpfphig_error*   Error );

#ifdef __cplusplus
}
#endif

#endif
