#ifndef FPHIG_PUBLISHER_UNSUBSCRIBE_H
#define FPHIG_PUBLISHER_UNSUBSCRIBE_H

#include "fphig/fphig.h"
#include "fphig/subscription.h"

fphig
fphig_publisher_unsubscribe( struct fphig_subscription*             Subscription,
                             FPHIG_OPTIONAL struct fphig_error*   Error );

#endif
