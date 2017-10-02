//
// Created by cui on 17-10-2.
//

#ifndef SB_SERVER_SB_FILTER_CHAIN_H
#define SB_SERVER_SB_FILTER_CHAIN_H

#include <sb_collection.h>
#include <server/sb_request.h>
#include <server/client/sb_client.h>

#define FILTER void*(*filter)(sb_client*,void*)

typedef struct{
    sb_arraylist filter_chain;
}sb_filter_chain;

int sb_add_filter(sb_filter_chain *filter_chain,FILTER);

int sb_init_filter_chain(sb_filter_chain *filter_chain);

#endif //SB_SERVER_SB_FILTER_CHAIN_H
