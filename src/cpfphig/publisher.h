#ifndef CPFPHIG_PUBLISHER_H
#define CPFPHIG_PUBLISHER_H

#include "cpfphig/cpfphig.h"
#include "cpfphig/subscription.h"
#include "cpfphig/thread_cond.h"
#include "cpfphig/list.h"
#include "cpfphig/mutex.h"

enum cpfphig_publisher_thread_cond_kind
{
    cpfphig_publisher_thread_cond_kind_ready,
    cpfphig_publisher_thread_cond_kind_abort,
    cpfphig_publisher_thread_cond_kind_data,
    cpfphig_publisher_thread_cond_kind_completed
};

struct cpfphig_subscription;

struct cpfphig_publisher
{
    // should be for dynamic data   "sequencer, track, note etc.."
    void*                                           data;

    // Allows only one publisher_publish at a time, cant subscribe when a publish is happening
    struct cpfphig_mutex                              mutex;
    struct cpfphig_list*                              subscriptions; // this could be local as well, but for now is allocated on heap so no big deal
    struct cpfphig_thread_cond                        publish_thread_cond;
    enum cpfphig_publisher_thread_cond_kind           published_thread_cond_kind;
    struct cpfphig_mutex                              broadcast_mutex;
    struct cpfphig_subscription*                      unsubscribing_subscription;
    struct cpfphig_mutex                              completed_mutex;
    struct cpfphig_thread_cond                        completed_thread_cond;
    enum cpfphig_publisher_thread_cond_kind           completed_thread_cond_kind;
    struct cpfphig_mutex                              completed_ack_mutex;
    struct cpfphig_thread_cond                        completed_ack_thread_cond;
    struct cpfphig_error                              published_error;
};

#define CPFPHIG_CONST_CPFPHIG_PUBLISHER { \
    NULL,                                   \
    CPFPHIG_CONST_CPFPHIG_MUTEX,              \
    NULL,                                   \
    CPFPHIG_CONST_CPFPHIG_THREAD_COND,        \
    cpfphig_publisher_thread_cond_kind_abort, \
    CPFPHIG_CONST_CPFPHIG_MUTEX,              \
    NULL,                                   \
    CPFPHIG_CONST_CPFPHIG_MUTEX,              \
    CPFPHIG_CONST_CPFPHIG_THREAD_COND,        \
    cpfphig_publisher_thread_cond_kind_abort, \
    CPFPHIG_CONST_CPFPHIG_MUTEX,              \
    CPFPHIG_CONST_CPFPHIG_THREAD_COND,        \
    CPFPHIG_CONST_CPFPHIG_ERROR               \
}

#endif
