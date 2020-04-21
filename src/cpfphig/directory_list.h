#ifndef DIRECTORY_LIST_H
#define DIRECTORY_LIST_H

#include "cpfphig/cpfphig.h"
#include "cpfphig/list.h"

cpfphig
cpfphig_directory_list( const char*                           Directory,
                      struct cpfphig_list*                    File_Names,
                      CPFPHIG_OPTIONAL struct cpfphig_error*  Error );

#endif
