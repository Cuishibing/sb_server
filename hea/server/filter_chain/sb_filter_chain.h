//
// Created by cui on 17-10-2.
//

#ifndef SB_SERVER_SB_FILTER_CHAIN_H
#define SB_SERVER_SB_FILTER_CHAIN_H

#include <sb_collection.h>
#include <server/sb_request.h>
#include <server/client/sb_client.h>

/*
 * 定义过滤器链,是过滤器的一个集合
 * */

/**
 * 过滤器(sb_client类型专用的),实际上是一个函数指针
 * */
#define FILTER void*(*filter)(sb_client*,void*)

/**
 * 过滤器(通用的),实际上是一个函数指针
 * */
#define FILTER_1 void*(*filter)(void*)

#define DEFAULT_FILTER_TYPE -1
#define FILTER_TYPE_0 0
#define FILTER_TYPE_1 1

typedef struct{
    sb_arraylist filter_chain;
    int filter_type;//过滤器的类别
}sb_filter_chain;

/**
 * 向过滤器链中添加一个过滤器
 * @param filter_chain 过滤链
 * @param filter 过滤器
 * @return 是否成功
 * */
int sb_add_filter(sb_filter_chain *filter_chain,FILTER);

int sb_add_filter_1(sb_filter_chain *filter_chain,FILTER_1);

/**
 * 初始化一个过滤器链
 * @param filter_chain 要被初始化的过滤器链
 * @return 是否成功
 * */
int sb_init_filter_chain(sb_filter_chain *filter_chain);

/**
 * 调用这个过滤器链
 * @param filter_chain 要被调用的过滤器链
 * @param client
 * 约定:最后一个函数如果返回NULL表示出现了错误
 * */
void* sb_invoke_filter_chain(sb_filter_chain *filter_chain,sb_client *client,void* arg);

#endif //SB_SERVER_SB_FILTER_CHAIN_H