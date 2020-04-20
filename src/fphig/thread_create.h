#ifndef FPHIG_THREAD_CREATE_H
#define FPHIG_THREAD_CREATE_H

#include "fphig/fphig.h"
#include "fphig/thread.h"
#include "fphig/thread_attr.h"

fphig
fphig_thread_create( struct fphig_thread*                   Thread,
                     struct fphig_thread_attr*              Thread_Attr,
                     void* (*Start_Routine)(void*),
                     FPHIG_OPTIONAL void*                 Arg,
                     FPHIG_OPTIONAL struct fphig_error*   Error );
#endif
