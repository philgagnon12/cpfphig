#include "cpfphig/cpfphig.h"
#include "cpfphig/list.h"
#include "cpfphig_test/mock/list_mock.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

cpfphig
cpfphig_list_remove( struct cpfphig_list*                      List,
                   void*                                   Item,
                   CPFPHIG_OPTIONAL struct cpfphig_error*    Error )
{
    assert_non_null( List );

    // When dealing with lists, use real functions
    cpfphig ret = (cpfphig)mock();

    if( ret == CPFPHIG_OK )
    {
        // Expecting the real thing to work but if it doesnt, have to make sure CPFPHIG_FAIL is returned
        ret = real_cpfphig_list_remove( List,
                                      Item,
                                      Error );
    }
    else
    {
        if( Error != NULL )
        {
            Error->error_type = (int)mock();
        }
    }

    return ret;
}
