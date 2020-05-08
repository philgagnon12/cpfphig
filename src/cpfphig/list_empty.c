#include "cpfphig/cpfphig.h"
#include "cpfphig/list.h"

// REMARK removes all nodes from list
cpfphig
cpfphig_list_empty( struct cpfphig_list*                    List,
                    CPFPHIG_OPTIONAL struct cpfphig_error*  Error )
{
    cpfphig                 shift_ret    = CPFPHIG_FAIL;
    struct cpfphig_error    shift_error  = CPFPHIG_CONST_CPFPHIG_ERROR;

    void* item;

    // Assume ok
    shift_ret               = CPFPHIG_OK;
    shift_error.error_type  = cpfphig_ok;

    while( CPFPHIG_OK == ( shift_ret = cpfphig_list_shift( List,
                                                           &item,
                                                           &shift_error ) ) )
    {
        // Shift is enough to remove nodes
    }
    if( shift_error.error_type == cpfphig_system_error &&
        shift_ret              == CPFPHIG_FAIL )
    {
        if( Error != NULL )
            *Error = shift_error;

        return CPFPHIG_FAIL;
    }


    return CPFPHIG_OK;
}


