#ifndef MELPHIG_PUBLISHER_H
#define MELPHIG_PUBLISHER_H

#include "melphig/melphig.h"
#include "melphig/subscription.h"
#include "melphig/thread_cond.h"
#include "melphig/list.h"
#include "melphig/mutex.h"

enum mphig_publisher_thread_cond_kind
{
    mphig_publisher_thread_cond_kind_ready,
    mphig_publisher_thread_cond_kind_abort,
    mphig_publisher_thread_cond_kind_data,
    mphig_publisher_thread_cond_kind_completed
};

struct mphig_subscription;

struct mphig_publisher
{
    // should be for dynamic data   "sequencer, track, note etc.."
    void*                                           data;

    // Allows only one publisher_publish at a time, cant subscribe when a publish is happening
    struct mphig_mutex                              mutex;
    struct mphig_list*                              subscriptions; // this could be local as well, but for now is allocated on heap so no big deal
    struct mphig_thread_cond                        publish_thread_cond;
    enum mphig_publisher_thread_cond_kind           published_thread_cond_kind;
    struct mphig_mutex                              broadcast_mutex;
    struct mphig_subscription*                      unsubscribing_subscription;
    struct mphig_mutex                              completed_mutex;
    struct mphig_thread_cond                        completed_thread_cond;
    enum mphig_publisher_thread_cond_kind           completed_thread_cond_kind;
    struct mphig_mutex                              completed_ack_mutex;
    struct mphig_thread_cond                        completed_ack_thread_cond;
    struct mphig_error                              published_error;
};

#define MELPHIG_CONST_MPHIG_PUBLISHER { \
    NULL,                                   \
    MELPHIG_CONST_MPHIG_MUTEX,              \
    NULL,                                   \
    MELPHIG_CONST_MPHIG_THREAD_COND,        \
    mphig_publisher_thread_cond_kind_abort, \
    MELPHIG_CONST_MPHIG_MUTEX,              \
    NULL,                                   \
    MELPHIG_CONST_MPHIG_MUTEX,              \
    MELPHIG_CONST_MPHIG_THREAD_COND,        \
    mphig_publisher_thread_cond_kind_abort, \
    MELPHIG_CONST_MPHIG_MUTEX,              \
    MELPHIG_CONST_MPHIG_THREAD_COND,        \
    MELPHIG_CONST_MPHIG_ERROR               \
}

#endif
