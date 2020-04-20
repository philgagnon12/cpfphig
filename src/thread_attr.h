#ifndef MELPHIG_THREAD_ATTR_H
#define MELPHIG_THREAD_ATTR_H

#include "melphig/melphig.h"

#ifdef MELPHIG_HAVE_PTHREAD_H
#include <pthread.h>
#endif

struct mphig_thread_attr
{

#ifdef MELPHIG_HAVE_PTHREAD_H
    pthread_attr_t   pthread_attr;
#endif

};

#ifdef MELPHIG_HAVE_PTHREAD_H
#define MELPHIG_CONST_MPHIG_THREAD_ATTR { 0 }
#endif

#endif
