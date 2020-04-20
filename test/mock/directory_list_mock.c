#include "melphig/melphig.h"
#include "melphig/directory_list.h"
#include "melphig/list.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

mphig
mphig_directory_list( const char*                           Directory,
                      struct mphig_list*                    File_Names,
                      MELPHIG_OPTIONAL struct mphig_error*  Error )
{
    struct mphig_list*  file_names = NULL;
    mphig               ret        = MELPHIG_FAIL;

    assert_non_null( Directory );
    assert_non_null( File_Names );

    ret = (mphig)mock();
    if( ret == MELPHIG_OK )
    {
        file_names = (struct mphig_list*)mock();

        *File_Names = *file_names;
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

