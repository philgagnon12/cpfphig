#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_PTHREAD_H

#include "cpfphig/thread_cond.h"
#include "cpfphig/mutex.h"

#include <pthread.h>
#include <errno.h>

#include <time.h>
#include <sys/time.h>

#include <math.h>

cpfphig
cpfphig_thread_cond_timed_wait( struct cpfphig_thread_cond*               Cond,
                              struct cpfphig_mutex*                     Mutex,
                              int                                     Milliseconds,
                              CPFPHIG_OPTIONAL struct cpfphig_error*    Error )
{
    cpfphig           ret                     = CPFPHIG_FAIL;
    struct timespec milliseconds_timespec   = { 0, 0 };
    int             timedwait_ret           = 0;
    struct timespec realtime                = { 0, 0 };

    // NULL checks
    if( Cond == NULL || Mutex == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message(cpfphig_system_error, "Cond or Mutex is NULL", Error, __FILE__, __FUNCTION__, __LINE__ );

        return CPFPHIG_FAIL;
    }

    if( Milliseconds > 0 )
    {
        milliseconds_timespec.tv_sec = (time_t)floor(Milliseconds / 1000);
        milliseconds_timespec.tv_nsec = ( Milliseconds * 1e6 ) - ( milliseconds_timespec.tv_sec * 1e9 );
    }

    // TODO need these , fix properly
    clock_gettime(CLOCK_REALTIME, &realtime ); // TODO check error

    milliseconds_timespec.tv_sec += realtime.tv_sec;
    milliseconds_timespec.tv_nsec += realtime.tv_nsec; // TODO i might bust limit

    timedwait_ret = pthread_cond_timedwait( Cond->pthread_cond,
                                            Mutex->pthread_mutex,
                                            &milliseconds_timespec );
    switch( timedwait_ret )
    {
        case 0:
            ret = CPFPHIG_OK;
            break;
        case ETIMEDOUT:
            if( Error != NULL )
                cpfphig_error_message( cpfphig_user_error, "pthread_cond_timedwait time exceeded", Error, __FILE__, __FUNCTION__, __LINE__ );

            ret = CPFPHIG_FAIL;
            break;
        default:
            if( Error != NULL )
                cpfphig_error_message( cpfphig_user_error, "pthread_cond_timedwait failed", Error, __FILE__, __FUNCTION__, __LINE__ );

            ret = CPFPHIG_FAIL;
    }

    return ret;
}

#endif
