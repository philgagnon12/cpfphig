#ifndef FPHIG_THREAD_JOIN_H
#define FPHIG_THREAD_JOIN_H

#include "melphig/melphig.h"
#include "melphig/thread.h"

fphig
fphig_thread_join( struct fphig_thread*                 Thread,
                   FPHIG_OPTIONAL void**              Value_Ptr,
                   FPHIG_OPTIONAL struct fphig_error* Error );

#endif
