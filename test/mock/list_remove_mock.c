#include "fphig/fphig.h"
#include "fphig/list.h"
#include "fphig_test/mock/list_mock.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

fphig
fphig_list_remove( struct fphig_list*                      List,
                   void*                                   Item,
                   FPHIG_OPTIONAL struct fphig_error*    Error )
{
    assert_non_null( List );

    // When dealing with lists, use real functions
    fphig ret = (fphig)mock();

    if( ret == FPHIG_OK )
    {
        // Expecting the real thing to work but if it doesnt, have to make sure FPHIG_FAIL is returned
        ret = real_fphig_list_remove( List,
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
