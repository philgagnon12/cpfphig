#ifndef LIST_MOCK_H
#define LIST_MOCK_H

#include "melphig/melphig.h"
#include "melphig/list.h"

mphig
real_mphig_list_push( struct mphig_list*                      List,
                      void*                                   Item,
                      MELPHIG_OPTIONAL struct mphig_error*    Error );

mphig
real_mphig_list_shift( struct mphig_list*                      List,
                       void*                                   Item,
                       MELPHIG_OPTIONAL struct mphig_error*    Error );

mphig
real_mphig_list_next( struct mphig_list_iterator* const       List_Iterator,
                      void*                                   Item,
                      MELPHIG_OPTIONAL struct mphig_error*    Error );

mphig
real_mphig_list_remove( struct mphig_list*                      List,
                        void*                                   Item,
                        MELPHIG_OPTIONAL struct mphig_error*    Error );

mphig
real_mphig_list_copy( struct mphig_list                       List,
                      struct mphig_list*                      List_Copy,
                      MELPHIG_OPTIONAL struct mphig_error*    Error );

#endif
