#ifndef MELPHIG_MUTEX_INIT_H
#define MELPHIG_MUTEX_INIT_H

#include "melphig/melphig.h"
#include "melphig/mutex.h"
#include "melphig/mutex_attr.h"

mphig
mphig_mutex_init( struct mphig_mutex*                    Mutex,
                  struct mphig_mutex_attr*               Mutex_Attr,
                  MELPHIG_OPTIONAL struct mphig_error*   Error );

#endif
