#ifndef CPFPHIG_SUBSCRIPTION_H
#define CPFPHIG_SUBSCRIPTION_H

#include "cpfphig/cpfphig.h"
#include "cpfphig/publisher.h"
#include "cpfphig/thread.h"
#include "cpfphig/thread_cond.h"


typedef cpfphig (cpfphig_subscription_routine_symbol)( void*                                   Publisher_Data,
                                                       void*                                   Subscription_Data,
                                                       CPFPHIG_OPTIONAL struct cpfphig_error*  Error);

struct cpfphig_publisher;

struct cpfphig_subscription
{
    // should be for const data like "subsription on a driver"
    void*                                           data;
    cpfphig_subscription_routine_symbol*            subscription_routine;
    struct cpfphig_publisher*                       publisher;
    struct cpfphig_thread                           thread;
    struct cpfphig_thread_cond                      ready_thread_cond;
    int                                             thread_ready;
};

#define CPFPHIG_CONST_CPFPHIG_SUBSCRIPTION { \
    NULL,                               \
    NULL,                               \
    NULL,                               \
    CPFPHIG_CONST_CPFPHIG_THREAD,         \
    CPFPHIG_CONST_CPFPHIG_THREAD_COND,    \
    0,                                  \
}

#endif
