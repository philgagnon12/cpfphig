#ifndef FPHIG_SUBSCRIPTION_H
#define FPHIG_SUBSCRIPTION_H

#include "fphig/fphig.h"
#include "fphig/publisher.h"
#include "fphig/thread.h"
#include "fphig/thread_cond.h"


typedef fphig (fphig_subscription_routine_symbol)( void*                                   Publisher_Data,
                                                   void*                                   Subscription_Data,
                                                   FPHIG_OPTIONAL struct fphig_error*    Error);

struct fphig_publisher;

struct fphig_subscription
{
    // should be for const data like "subsription on a driver"
    void*                                           data;
    fphig_subscription_routine_symbol*              subscription_routine;
    struct fphig_publisher*                         publisher;
    struct fphig_thread                             thread;
    struct fphig_thread_cond                        ready_thread_cond;
    int                                             thread_ready;
};

#define FPHIG_CONST_MPHIG_SUBSCRIPTION { \
    NULL,                               \
    NULL,                               \
    NULL,                               \
    FPHIG_CONST_MPHIG_THREAD,         \
    FPHIG_CONST_MPHIG_THREAD_COND,    \
    0,                                  \
}

#endif
