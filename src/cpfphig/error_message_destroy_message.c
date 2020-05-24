#include "cpfphig/cpfphig.h"
#include "cpfphig/error_message_destroy_message.h"
#include "cpfphig/free.h"

cpfphig
cpfphig_error_message_destroy_message( struct cpfphig_error* Error )
{
    // NULL checks
    if( Error == NULL )
        return CPFPHIG_FAIL;

    switch( Error->error_component_type )
    {
        case cpfphig_error_allocated_message:
                Error->error_component.allocated_message.message_len = 0;
                return cpfphig_free( &Error->error_component.allocated_message.message, NULL );
            break;
        default:
            return CPFPHIG_FAIL;
    }

    return CPFPHIG_OK;
}
