#include "melphig/melphig.h"
#include "melphig/destroy_directory_list.h"
#include "melphig_test/mock/destroy_directory_list_mock.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

mphig
mphig_destroy_directory_list( struct mphig_list*                    File_Names,
                              MELPHIG_OPTIONAL struct mphig_error*  Error )
{
    // Call the real function to prevent memory leaks in tests
    mphig ret = (mphig)mock();

    if( ret == MELPHIG_OK )
    {
        // Expecting the real thing to work but if it doesnt, have to make sure MELPHIG_FAIL is returned
        ret = real_mphig_destroy_directory_list( File_Names,
                                                 Error );
    }

    return ret;
}

