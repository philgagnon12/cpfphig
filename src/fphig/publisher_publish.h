#ifndef FPHIG_PUBLISHER_PUBLISH_H
#define FPHIG_PUBLISHER_PUBLISH_H

#include "fphig/fphig.h"
#include "fphig/publisher.h"

fphig
fphig_publisher_publish( struct fphig_publisher*                     Publisher,
                         void*                                       Data,
                         enum fphig_publisher_thread_cond_kind       Publish_Thread_Cond_Kind,
                         struct fphig_error*                         Error );
#endif
