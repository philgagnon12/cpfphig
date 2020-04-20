#include "fphig/fphig.h"
#include "fphig/list.h"
#include "fphig_test/mock/list_mock.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

fphig
fphig_list_next( struct fphig_list_iterator* const       List_Iterator,
                 void*                                   Item,
                 FPHIG_OPTIONAL struct fphig_error*    Error )
{
    assert_non_null( List_Iterator );

    // When dealing with lists, use real functions
    fphig ret = (fphig)mock();

    if( ret == FPHIG_OK )
    {
        // Expecting the real thing to work but if it doesnt, have to make sure FPHIG_FAIL is returned
        ret = real_fphig_list_next( List_Iterator,
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
