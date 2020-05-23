#ifndef CPFPHIG_PUBLISHER_PUBLISH_H
#define CPFPHIG_PUBLISHER_PUBLISH_H

#include "cpfphig/cpfphig.h"
#include "cpfphig/publisher.h"

cpfphig
cpfphig_publisher_publish( struct cpfphig_publisher*                Publisher,
                           void*                                    Data,
                           enum cpfphig_publisher_thread_cond_kind  Publish_Thread_Cond_Kind,
                           struct cpfphig_error*                    Error );
#endif
