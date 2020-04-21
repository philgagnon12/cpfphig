#include "cpfphig/cpfphig.h"
#include "cpfphig/destroy_directory_list.h"
#include "cpfphig_test/mock/destroy_directory_list_mock.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

cpfphig
cpfphig_destroy_directory_list( struct cpfphig_list*                    File_Names,
                              CPFPHIG_OPTIONAL struct cpfphig_error*  Error )
{
    // Call the real function to prevent memory leaks in tests
    cpfphig ret = (cpfphig)mock();

    if( ret == CPFPHIG_OK )
    {
        // Expecting the real thing to work but if it doesnt, have to make sure CPFPHIG_FAIL is returned
        ret = real_cpfphig_destroy_directory_list( File_Names,
                                                 Error );
    }

    return ret;
}

