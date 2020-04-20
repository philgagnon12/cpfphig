#ifndef FPHIG_PUBLISHER_SUBSCRIBE_H
#define FPHIG_PUBLISHER_SUBSCRIBE_H

#include "fphig/fphig.h"
#include "fphig/publisher.h"
#include "fphig/subscription.h"

fphig
fphig_publisher_subscribe( struct fphig_publisher*                      Publisher,
                           void*                                        Data, // TODO put that after routine thats how thread do it
                           fphig_subscription_routine_symbol*           Subscription_Routine,
                           struct fphig_subscription**                  Subscription,
                           FPHIG_OPTIONAL struct fphig_error*         Error );

#endif
