#ifndef FPHIG_PUBLISHER_H
#define FPHIG_PUBLISHER_H

#include "fphig/fphig.h"
#include "fphig/subscription.h"
#include "fphig/thread_cond.h"
#include "fphig/list.h"
#include "fphig/mutex.h"

enum fphig_publisher_thread_cond_kind
{
    fphig_publisher_thread_cond_kind_ready,
    fphig_publisher_thread_cond_kind_abort,
    fphig_publisher_thread_cond_kind_data,
    fphig_publisher_thread_cond_kind_completed
};

struct fphig_subscription;

struct fphig_publisher
{
    // should be for dynamic data   "sequencer, track, note etc.."
    void*                                           data;

    // Allows only one publisher_publish at a time, cant subscribe when a publish is happening
    struct fphig_mutex                              mutex;
    struct fphig_list*                              subscriptions; // this could be local as well, but for now is allocated on heap so no big deal
    struct fphig_thread_cond                        publish_thread_cond;
    enum fphig_publisher_thread_cond_kind           published_thread_cond_kind;
    struct fphig_mutex                              broadcast_mutex;
    struct fphig_subscription*                      unsubscribing_subscription;
    struct fphig_mutex                              completed_mutex;
    struct fphig_thread_cond                        completed_thread_cond;
    enum fphig_publisher_thread_cond_kind           completed_thread_cond_kind;
    struct fphig_mutex                              completed_ack_mutex;
    struct fphig_thread_cond                        completed_ack_thread_cond;
    struct fphig_error                              published_error;
};

#define FPHIG_CONST_MPHIG_PUBLISHER { \
    NULL,                                   \
    FPHIG_CONST_MPHIG_MUTEX,              \
    NULL,                                   \
    FPHIG_CONST_MPHIG_THREAD_COND,        \
    fphig_publisher_thread_cond_kind_abort, \
    FPHIG_CONST_MPHIG_MUTEX,              \
    NULL,                                   \
    FPHIG_CONST_MPHIG_MUTEX,              \
    FPHIG_CONST_MPHIG_THREAD_COND,        \
    fphig_publisher_thread_cond_kind_abort, \
    FPHIG_CONST_MPHIG_MUTEX,              \
    FPHIG_CONST_MPHIG_THREAD_COND,        \
    FPHIG_CONST_MPHIG_ERROR               \
}

#endif
