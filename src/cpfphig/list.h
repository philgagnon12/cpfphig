#ifndef CPFPHIG_LIST_H
#define CPFPHIG_LIST_H

#include "cpfphig/cpfphig.h"

struct cpfphig_list_node;
struct cpfphig_list_node
{
    struct cpfphig_list_node*   next;
    void*                       item;
    struct cpfphig_list_node*   previous;
};

#define CPFPHIG_CONST_CPFPHIG_LIST_NODE { NULL, NULL, NULL }

struct cpfphig_list
{
    struct cpfphig_list_node* first;
    struct cpfphig_list_node* last;

};

#define CPFPHIG_CONST_CPFPHIG_LIST { NULL, NULL }

struct cpfphig_list_iterator
{
    struct cpfphig_list*                list;
    struct cpfphig_list_node*           current_node;
};

#define CPFPHIG_CONST_CPFPHIG_LIST_ITERATOR { \
    NULL,   \
    NULL   \
}

cpfphig
cpfphig_list_push( struct cpfphig_list*                     List, // TODO add const ? 
                   void*                                    Item,
                   CPFPHIG_OPTIONAL struct cpfphig_error*   Error );

// REMARK cpfphig_list_shift will allow Item to come back as NULL, since an Item could have been manually free'd outside of cpfphig_list_* operations
cpfphig
cpfphig_list_shift( struct cpfphig_list*                    List,
                    void*                                   Item,
                    CPFPHIG_OPTIONAL struct cpfphig_error*  Error );

cpfphig
cpfphig_list_unshift( struct cpfphig_list*                    List,
                      void*                                   Item,
                      CPFPHIG_OPTIONAL struct cpfphig_error*  Error );


// IMPORTANT, Argument Ptr is declared as void*, but is handle as void** internally
// Declared as void* to avoid compiler warnings
// HOW TO USE
// struct cpfphig_list_iterator list_iterator = { list, NULL };
// int* item = NULL;
// cpfphig_list_next( &list_iterator, &item, NULL );
cpfphig
cpfphig_list_next( struct cpfphig_list_iterator* const      List_Iterator,
                   void*                                    Item,
                   CPFPHIG_OPTIONAL struct cpfphig_error*   Error );

// IMPORTANT, Argument Ptr is declared as void*, but is handle as void** internally
// Declared as void* to avoid compiler warnings
// HOW TO USE
// struct cpfphig_list_iterator list_iterator = { list, NULL };
// int* item = NULL;
// cpfphig_list_previous( &list_iterator, &item, NULL );
cpfphig
cpfphig_list_previous( struct cpfphig_list_iterator* const      List_Iterator,
                       void*                                    Item,
                       CPFPHIG_OPTIONAL struct cpfphig_error*   Error );

// IMPORTANT order of list will be changed after remove
// Is achieved through pointer equality
cpfphig
cpfphig_list_remove( struct cpfphig_list*                      List,
                     void*                                     Item,
                     CPFPHIG_OPTIONAL struct cpfphig_error*    Error );

// IMPORTANT it copies nodes only
cpfphig
cpfphig_list_copy( struct cpfphig_list                       List,
                   struct cpfphig_list*                      List_Copy,
                   CPFPHIG_OPTIONAL struct cpfphig_error*    Error );

// REMARK removes all nodes from list
cpfphig
cpfphig_list_empty( struct cpfphig_list*                    List,
                    CPFPHIG_OPTIONAL struct cpfphig_error*  Error );

#endif
