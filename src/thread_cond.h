#ifndef MELPHIG_THREAD_COND_H
#define MELPHIG_THREAD_COND_H

#include "melphig/melphig.h"

#ifdef MELPHIG_HAVE_PTHREAD_H
#include <pthread.h>
#endif

struct mphig_thread_cond
{

#ifdef MELPHIG_HAVE_PTHREAD_H
    pthread_cond_t* pthread_cond;
#endif

};

#ifdef MELPHIG_HAVE_PTHREAD_H
#define MELPHIG_CONST_MPHIG_THREAD_COND { NULL }
#endif

#endif
