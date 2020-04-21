#ifndef CPFPHIG_THREAD_ATTR_H
#define CPFPHIG_THREAD_ATTR_H

#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_PTHREAD_H
#include <pthread.h>
#endif

struct cpfphig_thread_attr
{

#ifdef CPFPHIG_HAVE_PTHREAD_H
    pthread_attr_t   pthread_attr;
#endif

};

#ifdef CPFPHIG_HAVE_PTHREAD_H
#define CPFPHIG_CONST_CPFPHIG_THREAD_ATTR { 0 }
#endif

#endif
