#ifndef MELPHIG_SUBSCRIPTION_H
#define MELPHIG_SUBSCRIPTION_H

#include "melphig/melphig.h"
#include "melphig/publisher.h"
#include "melphig/thread.h"
#include "melphig/thread_cond.h"


typedef mphig (mphig_subscription_routine_symbol)( void*                                   Publisher_Data,
                                                   void*                                   Subscription_Data,
                                                   MELPHIG_OPTIONAL struct mphig_error*    Error);

struct mphig_publisher;

struct mphig_subscription
{
    // should be for const data like "subsription on a driver"
    void*                                           data;
    mphig_subscription_routine_symbol*              subscription_routine;
    struct mphig_publisher*                         publisher;
    struct mphig_thread                             thread;
    struct mphig_thread_cond                        ready_thread_cond;
    int                                             thread_ready;
};

#define MELPHIG_CONST_MPHIG_SUBSCRIPTION { \
    NULL,                               \
    NULL,                               \
    NULL,                               \
    MELPHIG_CONST_MPHIG_THREAD,         \
    MELPHIG_CONST_MPHIG_THREAD_COND,    \
    0,                                  \
}

#endif
