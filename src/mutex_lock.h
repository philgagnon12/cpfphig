#ifndef FPHIG_MUTEX_LOCK_H
#define FPHIG_MUTEX_LOCK_H

#include "melphig/melphig.h"
#include "melphig/mutex.h"

fphig
fphig_mutex_lock( struct fphig_mutex*                    Mutex,
                  FPHIG_OPTIONAL struct fphig_error*   Error );

#endif
