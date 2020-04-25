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

#ifdef CPFPHIG_HAVE_WINDOWS_H
    int c_require_one_member;
#endif

};

#define CPFPHIG_CONST_CPFPHIG_MUTEX_ATTR { 0 }
#endif
