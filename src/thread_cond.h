#ifndef FPHIG_THREAD_COND_H
#define FPHIG_THREAD_COND_H

#include "melphig/melphig.h"

#ifdef FPHIG_HAVE_PTHREAD_H
#include <pthread.h>
#endif

struct fphig_thread_cond
{

#ifdef FPHIG_HAVE_PTHREAD_H
    pthread_cond_t* pthread_cond;
#endif

};

#ifdef FPHIG_HAVE_PTHREAD_H
#define FPHIG_CONST_MPHIG_THREAD_COND { NULL }
#endif

#endif
