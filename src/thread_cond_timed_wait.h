#ifndef FPHIG_THREAD_COND_TIMED_WAIT_H
#define FPHIG_THREAD_COND_TIMED_WAIT_H

fphig
fphig_thread_cond_timed_wait( struct fphig_thread_cond*               Cond,
                              struct fphig_mutex*                     Mutex,
                              int                                     Milliseconds,
                              FPHIG_OPTIONAL struct fphig_error*    Error );

#endif
