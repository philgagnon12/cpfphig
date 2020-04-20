#ifndef FPHIG_THREAD_H
#define FPHIG_THREAD_H

#include "fphig/fphig.h"

#ifdef FPHIG_HAVE_PTHREAD_H
#include <pthread.h>
#endif

struct fphig_thread
{

#ifdef FPHIG_HAVE_PTHREAD_H
    pthread_t   pthread;
#endif

};

#ifdef FPHIG_HAVE_PTHREAD_H
#define FPHIG_CONST_MPHIG_THREAD { 0 }
#endif

#endif
