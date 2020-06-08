#ifndef CPFPHIG_BINARY_TREE_H
#define CPFPHIG_BINARY_TREE_H

#include "cpfphig/cpfphig.h"

struct cpfphig_binary_search_tree;

struct cpfphig_binary_search_tree
{
    struct cpfphig_binary_search_tree*  left;
    struct cpfphig_binary_search_tree*  parent;
    struct cpfphig_binary_search_tree*  right;
    void*                               key;
    void*                               item;
};

#define CPFPHIG_CONST_CPFPHIG_BINARY_SEARCH_TREE { \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
}

typedef cpfphig (cpfphig_binary_search_tree_compare_symbol)( void*                     key,
                                                             void*                     tree_key,
                                                             int*                      result,
                                                             CPFPHIG_OPTIONAL struct   cpfphig_error* Error );

cpfphig
cpfphig_binary_search_tree_insert( struct cpfphig_binary_search_tree*                       Tree,
                                   void*                                                    Key,
                                   void*                                                    Item,
                                   cpfphig_binary_search_tree_compare_symbol*               Compare_Symbol,
                                   CPFPHIG_OPTIONAL struct cpfphig_binary_search_tree**     Tree_At_Key,
                                   CPFPHIG_OPTIONAL struct cpfphig_error*                   Error );




cpfphig
cpfphig_binary_search_tree_find( struct cpfphig_binary_search_tree*         Tree,
                                 void*                                      Key,
                                 void**                                     Item,
                                 cpfphig_binary_search_tree_compare_symbol* Compare_Symbol,
                                 CPFPHIG_OPTIONAL struct cpfphig_error*     Error );

cpfphig
cpfphig_binary_search_tree_find_tree( struct cpfphig_binary_search_tree*         Tree,
                                      void*                                      Key,
                                      struct cpfphig_binary_search_tree**        Tree_At_Key,
                                      cpfphig_binary_search_tree_compare_symbol* Compare_Symbol,
                                      CPFPHIG_OPTIONAL struct cpfphig_error*     Error );

// IF we wouldnt want to alter Dest, deep_copy() the tree
// could also have a int force, to not stop when a key already exists in Dest
cpfphig
cpfphig_binary_search_tree_merge( struct cpfphig_binary_search_tree*            Tree_Dest,
                                  struct cpfphig_binary_search_tree*            Tree_Src,
                                  cpfphig_binary_search_tree_compare_symbol*    Compare_Symbol,
                                  CPFPHIG_OPTIONAL struct cpfphig_error*        Error );

cpfphig
cpfphig_binary_search_tree_empty( struct cpfphig_binary_search_tree*            Tree,
                                  CPFPHIG_OPTIONAL struct cpfphig_error*        Error );



cpfphig
cpfphig_binary_search_tree_remove( struct cpfphig_binary_search_tree*         Tree,
                                   void*                                      Key,
                                   cpfphig_binary_search_tree_compare_symbol* Compare_Symbol,
                                   CPFPHIG_OPTIONAL void**                    Item,
                                   CPFPHIG_OPTIONAL struct cpfphig_error*     Error );




#endif
