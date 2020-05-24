#include "cpfphig/cpfphig.h"
#include "cpfphig/error_message_get_message.h"

cpfphig
cpfphig_error_message_get_message( struct cpfphig_error*    Error,
                                   const char**             Message,
                                   int*                     Message_Len )
{
    const char* message     = NULL;
    int         message_len = 0;

    // NULL checks
    if( Error == NULL || Message == NULL )
        return CPFPHIG_FAIL;

    switch( Error->error_component_type )
    {
        case cpfphig_error_allocated_message:
                message     = Error->error_component.allocated_message.message;
                message_len = Error->error_component.allocated_message.message_len;
            break;
        default:
            return CPFPHIG_FAIL;
    }

    *Message = message;

    if( Message_Len != NULL )
        *Message_Len = message_len;

    return CPFPHIG_OK;
}
