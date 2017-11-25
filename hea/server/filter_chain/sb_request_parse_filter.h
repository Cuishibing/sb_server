//
// Created by cui on 17-9-21.
//
#ifndef SB_SERVER_SB_REQUEST_PARSE_FILTER_H
#define SB_SERVER_SB_REQUEST_PARSE_FILTER_H

#include <sb_collection.h>
#include <stdio.h>
#include "filter_chain/sb_filter_chain.h"
#include "sb_client.h"
#include "sb_request.h"

/**
 * 定义请求解析模块
 *
 * 请求的解析使用一个过滤器链完成,整个过滤器链使用一个[sb_client]类型的对象作为参数,
 * 用于从中获取请求的数据.整个过滤器链调用完闭后将解析后的结果也存放在[sb_client]中.
 * */

/**
 * 解析请求所使用的过滤器链
 * */
static sb_filter_chain *request_parser = NULL;

/**
 * 设置解析请求所使用的过滤器链
 * @param filter_chain 过滤器链
 * @return 是否成功
 * */
int sb_set_request_parser(sb_filter_chain *filter_chain);

/**
 * 向过滤器链中添加一个过滤器
 * @param filter 要添加的过滤器
 * @return 是否成功
 * */
int sb_add_filter_request_parser(FILTER);

/**
 * 初始化过滤器链
 * @return 是否成功
 * */
int sb_init_request_parser();

/**
 * 获取请求解析过滤器链
 * @return 过滤器链
 * */
sb_filter_chain* sb_get_request_parser();

/**
 * 调用这个过滤器链
 * @param client 要被解析的请求
 * @return 是否成功
 * */
int invoke_request_parser(sb_client *client);

static const char *REQUEST_TARGET = "request_target";

#endif //SB_SERVER_SB_REQUEST_PROCESS_FILTER_H