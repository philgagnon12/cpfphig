#include "cpfphig/cpfphig.h"

#ifdef CPFPHIG_HAVE_WINDOWS_H

#include "cpfphig/thread_cond.h"
#include "cpfphig/mutex.h"

#include <windows.h>

cpfphig
cpfphig_thread_cond_timed_wait( struct cpfphig_thread_cond*               Cond,
                                struct cpfphig_mutex*                     Mutex,
                                int                                       Milliseconds,
                                CPFPHIG_OPTIONAL struct cpfphig_error*    Error )
{
    cpfphig           ret                           = CPFPHIG_FAIL;
    BOOL              sleep_condition_variable_ret  = 0;
    DWORD             last_error                    = 0;

    // NULL checks
    if( Cond == NULL || Mutex == NULL )
    {
        if( Error != NULL )
            cpfphig_error_message( cpfphig_system_error, "Cond or Mutex is NULL", Error );

        return CPFPHIG_FAIL;
    }

    sleep_condition_variable_ret = SleepConditionVariableCS( Cond->condition_variable,
                                                             Mutex->critical_section,
                                                             Milliseconds );

    // Assume ok
    ret = CPFPHIG_OK;

    if( sleep_condition_variable_ret == 0 )
    {
        last_error = GetLastError();

        switch( last_error )
        {
            case ERROR_TIMEOUT:
                if( Error != NULL )
                    cpfphig_error_message( cpfphig_user_error, "SleepConditionVariableCS time exceeded", Error );

                ret = CPFPHIG_FAIL;
                break;
            default:
                if( Error != NULL )
                    cpfphig_error_message( cpfphig_system_error, "SleepConditionVariableCS failed", Error );

                ret = CPFPHIG_FAIL;
        }
    }

    return ret;
}

#endif
