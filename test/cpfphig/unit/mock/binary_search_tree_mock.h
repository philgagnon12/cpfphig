#ifndef BINARY_SEARCH_TREE_MOCK_H
#define BINARY_SEARCH_TREE_MOCK_H

#include "cpfphig/cpfphig.h"
#include "cpfphig/binary_search_tree.h"

cpfphig
real_cpfphig_binary_search_tree_insert( struct cpfphig_binary_search_tree*         Tree,
                                        void*                                      Key,
                                        void*                                      Item,
                                        cpfphig_binary_search_tree_compare_symbol* Compare_Symbol,
                                        CPFPHIG_OPTIONAL struct cpfphig_error*     Error );




cpfphig
real_cpfphig_binary_search_tree_find( struct cpfphig_binary_search_tree*         Tree,
                                      void*                                      Key,
                                      void**                                     Item,
                                      cpfphig_binary_search_tree_compare_symbol* Compare_Symbol,
                                      CPFPHIG_OPTIONAL struct cpfphig_error*     Error );


#endif
