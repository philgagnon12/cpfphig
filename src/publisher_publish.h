#ifndef MELPHIG_PUBLISHER_PUBLISH_H
#define MELPHIG_PUBLISHER_PUBLISH_H

#include "melphig/melphig.h"
#include "melphig/publisher.h"

mphig
mphig_publisher_publish( struct mphig_publisher*                     Publisher,
                         void*                                       Data,
                         enum mphig_publisher_thread_cond_kind       Publish_Thread_Cond_Kind,
                         struct mphig_error*                         Error );
#endif
