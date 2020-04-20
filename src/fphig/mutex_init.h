#ifndef FPHIG_MUTEX_INIT_H
#define FPHIG_MUTEX_INIT_H

#include "fphig/fphig.h"
#include "fphig/mutex.h"
#include "fphig/mutex_attr.h"

fphig
fphig_mutex_init( struct fphig_mutex*                    Mutex,
                  struct fphig_mutex_attr*               Mutex_Attr,
                  FPHIG_OPTIONAL struct fphig_error*   Error );

#endif
