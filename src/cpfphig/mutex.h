#ifndef CPFPHIG_MUTEX_H
#define CPFPHIG_MUTEX_H

#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_PTHREAD_H
#include <pthread.h>
#endif

#ifdef CPFPHIG_HAVE_WINDOWS_H
#include <windows.h>
#endif

struct cpfphig_mutex
{

#ifdef CPFPHIG_HAVE_PTHREAD_H
    pthread_mutex_t*  pthread_mutex;
#endif

#ifdef CPFPHIG_HAVE_WINDOWS_H
    CRITICAL_SECTION* critical_section;
#endif

};

#ifdef CPFPHIG_HAVE_PTHREAD_H
#define CPFPHIG_CONST_CPFPHIG_MUTEX { NULL }
#endif

#ifdef CPFPHIG_HAVE_WINDOWS_H
#define CPFPHIG_CONST_CPFPHIG_MUTEX { NULL }
#endif

#endif
