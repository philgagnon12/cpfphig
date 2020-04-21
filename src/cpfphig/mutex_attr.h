#ifndef CPFPHIG_MUTEX_ATTR_H
#define CPFPHIG_MUTEX_ATTR_H

#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_PTHREAD_H
#include <pthread.h>
#endif

struct cpfphig_mutex_attr
{

#ifdef CPFPHIG_HAVE_PTHREAD_H
    pthread_mutexattr_t   pthread_mutex_attr;
#endif

};

#define CPFPHIG_CONST_CPFPHIG_MUTEX_ATTR {}
#endif
