#ifndef MELPHIG_THREAD_JOIN_H
#define MELPHIG_THREAD_JOIN_H

#include "melphig/melphig.h"
#include "melphig/thread.h"

mphig
mphig_thread_join( struct mphig_thread*                 Thread,
                   MELPHIG_OPTIONAL void**              Value_Ptr,
                   MELPHIG_OPTIONAL struct mphig_error* Error );

#endif
