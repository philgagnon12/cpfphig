#ifndef FPHIG_PUBLISHER_SUBSCRIBE_H
#define FPHIG_PUBLISHER_SUBSCRIBE_H

#include "melphig/melphig.h"
#include "melphig/publisher.h"
#include "melphig/subscription.h"

fphig
fphig_publisher_subscribe( struct fphig_publisher*                      Publisher,
                           void*                                        Data, // TODO put that after routine thats how thread do it
                           fphig_subscription_routine_symbol*           Subscription_Routine,
                           struct fphig_subscription**                  Subscription,
                           FPHIG_OPTIONAL struct fphig_error*         Error );

#endif
