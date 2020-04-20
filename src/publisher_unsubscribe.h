#ifndef MELPHIG_PUBLISHER_UNSUBSCRIBE_H
#define MELPHIG_PUBLISHER_UNSUBSCRIBE_H

#include "melphig/melphig.h"
#include "melphig/subscription.h"

mphig
mphig_publisher_unsubscribe( struct mphig_subscription*             Subscription,
                             MELPHIG_OPTIONAL struct mphig_error*   Error );

#endif
