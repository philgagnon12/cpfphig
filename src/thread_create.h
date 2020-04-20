#ifndef MELPHIG_THREAD_CREATE_H
#define MELPHIG_THREAD_CREATE_H

#include "melphig/melphig.h"
#include "melphig/thread.h"
#include "melphig/thread_attr.h"

mphig
mphig_thread_create( struct mphig_thread*                   Thread,
                     struct mphig_thread_attr*              Thread_Attr,
                     void* (*Start_Routine)(void*),
                     MELPHIG_OPTIONAL void*                 Arg,
                     MELPHIG_OPTIONAL struct mphig_error*   Error );
#endif
