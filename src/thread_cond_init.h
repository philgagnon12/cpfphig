#ifndef FPHIG_THREAD_COND_INIT_H
#define FPHIG_THREAD_COND_INIT_H

#include "melphig/melphig.h"
#include "melphig/thread_cond.h"
#include "melphig/thread_cond_attr.h"

fphig
fphig_thread_cond_init( struct fphig_thread_cond*               Cond,
                        struct fphig_thread_cond_attr*          Cond_Attr,
                        FPHIG_OPTIONAL struct fphig_error*    Error );

#endif
