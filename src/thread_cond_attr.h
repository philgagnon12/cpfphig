#ifndef MELPHIG_THREAD_COND_ATTR_H
#define MELPHIG_THREAD_COND_ATTR_H

#include "melphig/melphig.h"

#ifdef MELPHIG_HAVE_PTHREAD_H
#include <pthread.h>
#endif

struct mphig_thread_cond_attr
{

#ifdef MELPHIG_HAVE_PTHREAD_H
    pthread_condattr_t   pthread_cond_attr;
#endif

};

#ifdef MELPHIG_HAVE_PTHREAD_H
#define MELPHIG_CONST_MPHIG_THREAD_COND_ATTR { 0 }
#endif

#endif
