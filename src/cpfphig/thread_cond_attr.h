#ifndef CPFPHIG_THREAD_COND_ATTR_H
#define CPFPHIG_THREAD_COND_ATTR_H

#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_PTHREAD_H
#include <pthread.h>
#endif

struct cpfphig_thread_cond_attr
{

#ifdef CPFPHIG_HAVE_PTHREAD_H
    pthread_condattr_t   pthread_cond_attr;
#endif

#ifdef CPFPHIG_HAVE_WINDOWS_H
    int c_require_one_member;
#endif

};

#ifdef CPFPHIG_HAVE_PTHREAD_H
#define CPFPHIG_CONST_CPFPHIG_THREAD_COND_ATTR { 0 }
#endif

#ifdef CPFPHIG_HAVE_WINDOWS_H
#define CPFPHIG_CONST_CPFPHIG_THREAD_COND_ATTR { 0 }
#endif

#endif
