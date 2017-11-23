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

/*
 * 调用过滤器链中的函数
 * 约定:最后一个函数如果返回NULL表示出现了错误
 * */
void* sb_invoke_filter_chain(sb_filter_chain *filter_chain,sb_client *client,void* arg);

#endif //SB_SERVER_SB_FILTER_CHAIN_H