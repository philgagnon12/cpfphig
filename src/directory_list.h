#ifndef DIRECTORY_LIST_H
#define DIRECTORY_LIST_H

#include "melphig/melphig.h"
#include "melphig/list.h"

mphig
mphig_directory_list( const char*                           Directory,
                      struct mphig_list*                    File_Names,
                      MELPHIG_OPTIONAL struct mphig_error*  Error );

#endif
