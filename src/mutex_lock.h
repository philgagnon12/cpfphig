#ifndef MELPHIG_MUTEX_LOCK_H
#define MELPHIG_MUTEX_LOCK_H

#include "melphig/melphig.h"
#include "melphig/mutex.h"

mphig
mphig_mutex_lock( struct mphig_mutex*                    Mutex,
                  MELPHIG_OPTIONAL struct mphig_error*   Error );

#endif
