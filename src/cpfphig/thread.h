#ifndef CPFPHIG_THREAD_H
#define CPFPHIG_THREAD_H

#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_PTHREAD_H
#include <pthread.h>
#endif

#ifdef CPFPHIG_HAVE_WINDOWS_H
#include <windows.h>
#endif

struct cpfphig_thread
{

#ifdef CPFPHIG_HAVE_PTHREAD_H
    pthread_t   pthread;
#endif

#ifdef CPFPHIG_HAVE_WINDOWS_H
    HANDLE handle;
#endif

};

#ifdef CPFPHIG_HAVE_PTHREAD_H
#define CPFPHIG_CONST_CPFPHIG_THREAD { 0 }
#endif

#endif
