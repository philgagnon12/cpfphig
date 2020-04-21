#ifndef CPFPHIG_THREAD_COND_H
#define CPFPHIG_THREAD_COND_H

#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_PTHREAD_H
#include <pthread.h>
#endif

struct cpfphig_thread_cond
{

#ifdef CPFPHIG_HAVE_PTHREAD_H
    pthread_cond_t* pthread_cond;
#endif

};

#ifdef CPFPHIG_HAVE_PTHREAD_H
#define CPFPHIG_CONST_CPFPHIG_THREAD_COND { NULL }
#endif

#endif
