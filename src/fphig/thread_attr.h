#ifndef FPHIG_THREAD_ATTR_H
#define FPHIG_THREAD_ATTR_H

#include "fphig/fphig.h"

#ifdef FPHIG_HAVE_PTHREAD_H
#include <pthread.h>
#endif

struct fphig_thread_attr
{

#ifdef FPHIG_HAVE_PTHREAD_H
    pthread_attr_t   pthread_attr;
#endif

};

#ifdef FPHIG_HAVE_PTHREAD_H
#define FPHIG_CONST_FPHIG_THREAD_ATTR { 0 }
#endif

#endif
