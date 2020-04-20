#include "melphig/melphig.h"
#include "melphig/list.h"
#include "melphig_test/mock/list_mock.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

mphig
mphig_list_push( struct mphig_list*                      List,
                 void*                                   Item,
                 MELPHIG_OPTIONAL struct mphig_error*    Error )
{
    assert_non_null( List );

    // Call the real function to prevent memory leaks in tests
    // when adding malloc'ed items
    mphig ret = (mphig)mock();

    if( ret == MELPHIG_OK )
    {
        // Expecting the real thing to work but if it doesnt, have to make sure MELPHIG_FAIL is returned
        ret = real_mphig_list_push( List,
                                    Item,
                                    Error );
    }

    return ret;
}
