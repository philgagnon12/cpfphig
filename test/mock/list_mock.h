#ifndef LIST_MOCK_H
#define LIST_MOCK_H

#include "fphig/fphig.h"
#include "fphig/list.h"

fphig
real_fphig_list_push( struct fphig_list*                      List,
                      void*                                   Item,
                      FPHIG_OPTIONAL struct fphig_error*    Error );

fphig
real_fphig_list_shift( struct fphig_list*                      List,
                       void*                                   Item,
                       FPHIG_OPTIONAL struct fphig_error*    Error );

fphig
real_fphig_list_next( struct fphig_list_iterator* const       List_Iterator,
                      void*                                   Item,
                      FPHIG_OPTIONAL struct fphig_error*    Error );

fphig
real_fphig_list_remove( struct fphig_list*                      List,
                        void*                                   Item,
                        FPHIG_OPTIONAL struct fphig_error*    Error );

fphig
real_fphig_list_copy( struct fphig_list                       List,
                      struct fphig_list*                      List_Copy,
                      FPHIG_OPTIONAL struct fphig_error*    Error );

#endif
