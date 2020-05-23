#ifndef CPFPHIG_THREAD_COND_TIMED_WAIT_H
#define CPFPHIG_THREAD_COND_TIMED_WAIT_H

cpfphig
cpfphig_thread_cond_timed_wait( struct cpfphig_thread_cond*             Cond,
                                struct cpfphig_mutex*                   Mutex,
                                int                                     Milliseconds,
                                CPFPHIG_OPTIONAL struct cpfphig_error*  Error );

#endif
