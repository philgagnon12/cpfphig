#ifndef MELPHIG_PUBLISHER_SUBSCRIBE_H
#define MELPHIG_PUBLISHER_SUBSCRIBE_H

#include "melphig/melphig.h"
#include "melphig/publisher.h"
#include "melphig/subscription.h"

mphig
mphig_publisher_subscribe( struct mphig_publisher*                      Publisher,
                           void*                                        Data, // TODO put that after routine thats how thread do it
                           mphig_subscription_routine_symbol*           Subscription_Routine,
                           struct mphig_subscription**                  Subscription,
                           MELPHIG_OPTIONAL struct mphig_error*         Error );

#endif
