#ifndef MELPHIG_THREAD_COND_TIMED_WAIT_H
#define MELPHIG_THREAD_COND_TIMED_WAIT_H

mphig
mphig_thread_cond_timed_wait( struct mphig_thread_cond*               Cond,
                              struct mphig_mutex*                     Mutex,
                              int                                     Milliseconds,
                              MELPHIG_OPTIONAL struct mphig_error*    Error );

#endif
