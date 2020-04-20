#ifndef FPHIG_MUTEX_H
#define FPHIG_MUTEX_H

#include "fphig/fphig.h"

#ifdef FPHIG_HAVE_PTHREAD_H
#include <pthread.h>
#endif

struct fphig_mutex
{

#ifdef FPHIG_HAVE_PTHREAD_H
    pthread_mutex_t*  pthread_mutex;
#endif

};

#ifdef FPHIG_HAVE_PTHREAD_H
#define FPHIG_CONST_FPHIG_MUTEX { NULL }
#endif

#endif
