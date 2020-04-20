#include "melphig/melphig.h"
#include "melphig/list.h"
#include "melphig_test/mock/list_mock.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

fphig
fphig_list_copy( struct fphig_list                       List,
                 struct fphig_list*                      List_Copy,
                 FPHIG_OPTIONAL struct fphig_error*    Error )
{
    assert_non_null( List_Copy );

    // When dealing with lists, use real functions
    fphig ret = (fphig)mock();

    if( ret == FPHIG_OK )
    {
        // Expecting the real thing to work but if it doesnt, have to make sure FPHIG_FAIL is returned
        ret = real_fphig_list_copy( List,
                                    List_Copy,
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
