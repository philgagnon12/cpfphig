#ifndef CPFPHIG_PUBLISHER_SUBSCRIBE_H
#define CPFPHIG_PUBLISHER_SUBSCRIBE_H

#include "cpfphig/cpfphig.h"
#include "cpfphig/publisher.h"
#include "cpfphig/subscription.h"

cpfphig
cpfphig_publisher_subscribe( struct cpfphig_publisher*                  Publisher,
                             void*                                      Data, // TODO put that after routine thats how thread do it
                             cpfphig_subscription_routine_symbol*       Subscription_Routine,
                             struct cpfphig_subscription**              Subscription,
                             CPFPHIG_OPTIONAL struct cpfphig_error*     Error );

#endif
