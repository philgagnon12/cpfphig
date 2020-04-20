#ifndef MELPHIG_MODULES_LOAD_ALL_H
#define MELPHIG_MODULES_LOAD_ALL_H

#include "melphig/melphig.h"
#include "melphig/list.h"
#include "melphig/modules_load_all.h"

// You have to push your "module" pointers to the list
typedef mphig (mphig_module_load_symbol)( const char*                              File_Name,
                                          struct mphig_list*                       Modules,
                                          MELPHIG_OPTIONAL struct mphig_error*     Error );

mphig
mphig_modules_load_all( struct mphig_list                         Modules_Directories,
                        mphig_module_load_symbol*                 Mphig_Module_Load,
                        struct mphig_list*                        Modules,
                        MELPHIG_OPTIONAL struct mphig_error*      Error );

#endif
