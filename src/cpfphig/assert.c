#include "cpfphig/assert.h"

#include <stdio.h>

void
cpfphig_assert_failed( const char*    Message,
                       const char*    File,
                       const char*    Function,
                       size_t         Line )
{
    if( Message == NULL || File == NULL || Function == NULL )
    {
        fprintf( stderr, "%s(%s):%d; Message, File or Function is NULL !\n", __FILE__, __FUNCTION__, __LINE__ );

        assert( !( Message == NULL || File == NULL || Function == NULL ) );
    }
    fprintf( stderr, "%s(%s):%zu; %s !\n", File, Function, Line, Message );
}


