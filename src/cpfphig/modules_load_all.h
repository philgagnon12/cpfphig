#ifndef CPFPHIG_MODULES_LOAD_ALL_H
#define CPFPHIG_MODULES_LOAD_ALL_H

#include "cpfphig/cpfphig.h"
#include "cpfphig/list.h"
#include "cpfphig/modules_load_all.h"

// You have to push your "module" pointers to the list
typedef cpfphig (cpfphig_module_load_symbol)( const char*                                File_Name,
                                              struct cpfphig_list*                       Modules,
                                              CPFPHIG_OPTIONAL struct cpfphig_error*     Error );

cpfphig
cpfphig_modules_load_all( struct cpfphig_list                         Modules_Directories,
                          cpfphig_module_load_symbol*                 Cpfphig_Module_Load,
                          struct cpfphig_list*                        Modules,
                          CPFPHIG_OPTIONAL struct cpfphig_error*      Error );

#endif
