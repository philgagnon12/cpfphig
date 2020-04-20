#ifndef FPHIG_THREAD_COND_ATTR_H
#define FPHIG_THREAD_COND_ATTR_H

#include "fphig/fphig.h"

#ifdef FPHIG_HAVE_PTHREAD_H
#include <pthread.h>
#endif

struct fphig_thread_cond_attr
{

#ifdef FPHIG_HAVE_PTHREAD_H
    pthread_condattr_t   pthread_cond_attr;
#endif

};

#ifdef FPHIG_HAVE_PTHREAD_H
#define FPHIG_CONST_FPHIG_THREAD_COND_ATTR { 0 }
#endif

#endif
