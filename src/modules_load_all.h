#ifndef FPHIG_MODULES_LOAD_ALL_H
#define FPHIG_MODULES_LOAD_ALL_H

#include "melphig/melphig.h"
#include "melphig/list.h"
#include "melphig/modules_load_all.h"

// You have to push your "module" pointers to the list
typedef fphig (fphig_module_load_symbol)( const char*                              File_Name,
                                          struct fphig_list*                       Modules,
                                          FPHIG_OPTIONAL struct fphig_error*     Error );

fphig
fphig_modules_load_all( struct fphig_list                         Modules_Directories,
                        fphig_module_load_symbol*                 Mphig_Module_Load,
                        struct fphig_list*                        Modules,
                        FPHIG_OPTIONAL struct fphig_error*      Error );

#endif
