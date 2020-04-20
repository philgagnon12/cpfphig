#ifndef MELPHIG_THREAD_COND_INIT_H
#define MELPHIG_THREAD_COND_INIT_H

#include "melphig/melphig.h"
#include "melphig/thread_cond.h"
#include "melphig/thread_cond_attr.h"

mphig
mphig_thread_cond_init( struct mphig_thread_cond*               Cond,
                        struct mphig_thread_cond_attr*          Cond_Attr,
                        MELPHIG_OPTIONAL struct mphig_error*    Error );

#endif
