#ifndef MELPHIG_MUTEX_UNLOCK_H
#define MELPHIG_MUTEX_UNLOCK_H

#include "melphig/melphig.h"
#include "melphig/mutex.h"

mphig
mphig_mutex_unlock( struct mphig_mutex*                    Mutex,
                    MELPHIG_OPTIONAL struct mphig_error*   Error );

#endif
