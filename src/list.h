#ifndef MELPHIG_LIST_H
#define MELPHIG_LIST_H

#include "melphig/melphig.h"

struct mphig_list_node;
struct mphig_list_node
{
    struct mphig_list_node* next;
    void*                   item;
    struct mphig_list_node* previous;
};

#define MELPHIG_CONST_MPHIG_LIST_NODE { NULL, NULL, NULL }

struct mphig_list
{
    struct mphig_list_node* first;
    struct mphig_list_node* last;

};

#define MELPHIG_CONST_MPHIG_LIST { NULL, NULL }

struct mphig_list_iterator
{
    struct mphig_list*      list;
    struct mphig_list_node* current_node;

};

#define MELPHIG_CONST_MPHIG_LIST_ITERATOR { NULL, NULL }

mphig
mphig_list_push( struct mphig_list*                      List, // TODO add const ? 
                 void*                                   Item,
                 MELPHIG_OPTIONAL struct mphig_error*    Error );

// REMARK mphig_list_shift will allow Item to come back as NULL, since an Item could have been manually free'd outside of mphig_list_* operations
mphig
mphig_list_shift( struct mphig_list*                      List,
                  void*                                   Item,
                  MELPHIG_OPTIONAL struct mphig_error*    Error );


// IMPORTANT, Argument Ptr is declared as void*, but is handle as void** internally
// Declared as void* to avoid compiler warnings
// HOW TO USE
// struct mphig_list_iterator list_iterator = { list, NULL };
// int* item = NULL;
// mphig_list_next( &list_iterator, &item, NULL );
mphig
mphig_list_next( struct mphig_list_iterator* const       List_Iterator,
                 void*                                   Item,
                 MELPHIG_OPTIONAL struct mphig_error*    Error );

// IMPORTANT order of list will be changed after remove
// Is achieved through pointer equality
mphig
mphig_list_remove( struct mphig_list*                      List,
                   void*                                   Item,
                   MELPHIG_OPTIONAL struct mphig_error*    Error );

// IMPORTANT it copies nodes only
mphig
mphig_list_copy( struct mphig_list                       List,
                 struct mphig_list*                      List_Copy,
                 MELPHIG_OPTIONAL struct mphig_error*    Error );

#endif
