#ifndef FPHIG_LIST_H
#define FPHIG_LIST_H

#include "fphig/fphig.h"

struct fphig_list_node;
struct fphig_list_node
{
    struct fphig_list_node* next;
    void*                   item;
    struct fphig_list_node* previous;
};

#define FPHIG_CONST_MPHIG_LIST_NODE { NULL, NULL, NULL }

struct fphig_list
{
    struct fphig_list_node* first;
    struct fphig_list_node* last;

};

#define FPHIG_CONST_MPHIG_LIST { NULL, NULL }

struct fphig_list_iterator
{
    struct fphig_list*      list;
    struct fphig_list_node* current_node;

};

#define FPHIG_CONST_MPHIG_LIST_ITERATOR { NULL, NULL }

fphig
fphig_list_push( struct fphig_list*                      List, // TODO add const ? 
                 void*                                   Item,
                 FPHIG_OPTIONAL struct fphig_error*    Error );

// REMARK fphig_list_shift will allow Item to come back as NULL, since an Item could have been manually free'd outside of fphig_list_* operations
fphig
fphig_list_shift( struct fphig_list*                      List,
                  void*                                   Item,
                  FPHIG_OPTIONAL struct fphig_error*    Error );


// IMPORTANT, Argument Ptr is declared as void*, but is handle as void** internally
// Declared as void* to avoid compiler warnings
// HOW TO USE
// struct fphig_list_iterator list_iterator = { list, NULL };
// int* item = NULL;
// fphig_list_next( &list_iterator, &item, NULL );
fphig
fphig_list_next( struct fphig_list_iterator* const       List_Iterator,
                 void*                                   Item,
                 FPHIG_OPTIONAL struct fphig_error*    Error );

// IMPORTANT order of list will be changed after remove
// Is achieved through pointer equality
fphig
fphig_list_remove( struct fphig_list*                      List,
                   void*                                   Item,
                   FPHIG_OPTIONAL struct fphig_error*    Error );

// IMPORTANT it copies nodes only
fphig
fphig_list_copy( struct fphig_list                       List,
                 struct fphig_list*                      List_Copy,
                 FPHIG_OPTIONAL struct fphig_error*    Error );

#endif
