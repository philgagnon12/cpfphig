#ifndef LIST_MOCK_H
#define LIST_MOCK_H

#include "cpfphig/cpfphig.h"
#include "cpfphig/list.h"

cpfphig
real_cpfphig_list_push( struct cpfphig_list*                      List,
                      void*                                   Item,
                      CPFPHIG_OPTIONAL struct cpfphig_error*    Error );

cpfphig
real_cpfphig_list_shift( struct cpfphig_list*                      List,
                       void*                                   Item,
                       CPFPHIG_OPTIONAL struct cpfphig_error*    Error );

cpfphig
real_cpfphig_list_next( struct cpfphig_list_iterator* const       List_Iterator,
                      void*                                   Item,
                      CPFPHIG_OPTIONAL struct cpfphig_error*    Error );

cpfphig
real_cpfphig_list_remove( struct cpfphig_list*                      List,
                        void*                                   Item,
                        CPFPHIG_OPTIONAL struct cpfphig_error*    Error );

cpfphig
real_cpfphig_list_copy( struct cpfphig_list                       List,
                      struct cpfphig_list*                      List_Copy,
                      CPFPHIG_OPTIONAL struct cpfphig_error*    Error );

#endif
