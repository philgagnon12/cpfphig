#ifndef FPHIG_MUTEX_ATTR_H
#define FPHIG_MUTEX_ATTR_H

#include "fphig/fphig.h"

#ifdef FPHIG_HAVE_PTHREAD_H
#include <pthread.h>
#endif

struct fphig_mutex_attr
{

#ifdef FPHIG_HAVE_PTHREAD_H
    pthread_mutexattr_t   pthread_mutex_attr;
#endif

};

#define FPHIG_CONST_FPHIG_MUTEX_ATTR {}
#endif