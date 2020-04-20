#include "fphig/fphig.h"
#include "fphig/directory_list.h"
#include "fphig/list.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

fphig
fphig_directory_list( const char*                           Directory,
                      struct fphig_list*                    File_Names,
                      FPHIG_OPTIONAL struct fphig_error*  Error )
{
    struct fphig_list*  file_names = NULL;
    fphig               ret        = FPHIG_FAIL;

    assert_non_null( Directory );
    assert_non_null( File_Names );

    ret = (fphig)mock();
    if( ret == FPHIG_OK )
    {
        file_names = (struct fphig_list*)mock();

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

