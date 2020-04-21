#include "cpfphig/cpfphig.h"
#include "cpfphig/list.h"
#include "cpfphig/unit/mock/list_mock.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

cpfphig
cpfphig_list_push( struct cpfphig_list*                      List,
                 void*                                   Item,
                 CPFPHIG_OPTIONAL struct cpfphig_error*    Error )
{
    assert_non_null( List );

    // Call the real function to prevent memory leaks in tests
    // when adding malloc'ed items
    cpfphig ret = (cpfphig)mock();

    if( ret == CPFPHIG_OK )
    {
        // Expecting the real thing to work but if it doesnt, have to make sure CPFPHIG_FAIL is returned
        ret = real_cpfphig_list_push( List,
                                    Item,
                                    Error );
    }

    return ret;
}
