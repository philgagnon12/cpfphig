#ifndef MELPHIG_MUTEX_H
#define MELPHIG_MUTEX_H

#include "melphig/melphig.h"

#ifdef MELPHIG_HAVE_PTHREAD_H
#include <pthread.h>
#endif

struct mphig_mutex
{

#ifdef MELPHIG_HAVE_PTHREAD_H
    pthread_mutex_t*  pthread_mutex;
#endif

};

#ifdef MELPHIG_HAVE_PTHREAD_H
#define MELPHIG_CONST_MPHIG_MUTEX { NULL }
#endif

#endif
