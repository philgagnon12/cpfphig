#ifndef DIRECTORY_LIST_H
#define DIRECTORY_LIST_H

#include "fphig/fphig.h"
#include "fphig/list.h"

fphig
fphig_directory_list( const char*                           Directory,
                      struct fphig_list*                    File_Names,
                      FPHIG_OPTIONAL struct fphig_error*  Error );

#endif
