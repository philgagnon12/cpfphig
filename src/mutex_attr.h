#ifndef MELPHIG_MUTEX_ATTR_H
#define MELPHIG_MUTEX_ATTR_H

#include "melphig/melphig.h"

#ifdef MELPHIG_HAVE_PTHREAD_H
#include <pthread.h>
#endif

struct mphig_mutex_attr
{

#ifdef MELPHIG_HAVE_PTHREAD_H
    pthread_mutexattr_t   pthread_mutex_attr;
#endif

};

#define MELPHIG_CONST_MPHIG_MUTEX_ATTR {}
#endif
