#ifndef MELPHIG_THREAD_H
#define MELPHIG_THREAD_H

#include "melphig/melphig.h"

#ifdef MELPHIG_HAVE_PTHREAD_H
#include <pthread.h>
#endif

struct mphig_thread
{

#ifdef MELPHIG_HAVE_PTHREAD_H
    pthread_t   pthread;
#endif

};

#ifdef MELPHIG_HAVE_PTHREAD_H
#define MELPHIG_CONST_MPHIG_THREAD { 0 }
#endif

#endif
