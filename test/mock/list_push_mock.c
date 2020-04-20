#include "melphig/melphig.h"
#include "melphig/list.h"
#include "melphig_test/mock/list_mock.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

fphig
fphig_list_push( struct fphig_list*                      List,
                 void*                                   Item,
                 FPHIG_OPTIONAL struct fphig_error*    Error )
{
    assert_non_null( List );

    // Call the real function to prevent memory leaks in tests
    // when adding malloc'ed items
    fphig ret = (fphig)mock();

    if( ret == FPHIG_OK )
    {
        // Expecting the real thing to work but if it doesnt, have to make sure FPHIG_FAIL is returned
        ret = real_fphig_list_push( List,
                                    Item,
                                    Error );
    }

    return ret;
}
