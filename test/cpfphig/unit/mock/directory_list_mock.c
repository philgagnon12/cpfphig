#include "cpfphig/cpfphig.h"
#include "cpfphig/directory_list.h"
#include "cpfphig/list.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

cpfphig
cpfphig_directory_list( const char*                             Directory,
                        struct cpfphig_list*                    File_Names,
                        CPFPHIG_OPTIONAL struct cpfphig_error*  Error )
{
    struct cpfphig_list*  file_names = NULL;
    cpfphig               ret        = CPFPHIG_FAIL;

    assert_non_null( Directory );
    assert_non_null( File_Names );

    ret = (cpfphig)mock();
    if( ret == CPFPHIG_OK )
    {
        file_names = (struct cpfphig_list*)mock();

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

