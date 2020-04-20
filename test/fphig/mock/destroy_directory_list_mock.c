#include "fphig/fphig.h"
#include "fphig/destroy_directory_list.h"
#include "fphig_test/mock/destroy_directory_list_mock.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

fphig
fphig_destroy_directory_list( struct fphig_list*                    File_Names,
                              FPHIG_OPTIONAL struct fphig_error*  Error )
{
    // Call the real function to prevent memory leaks in tests
    fphig ret = (fphig)mock();

    if( ret == FPHIG_OK )
    {
        // Expecting the real thing to work but if it doesnt, have to make sure FPHIG_FAIL is returned
        ret = real_fphig_destroy_directory_list( File_Names,
                                                 Error );
    }

    return ret;
}

