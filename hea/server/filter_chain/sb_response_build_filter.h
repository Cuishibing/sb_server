//
// Created by cui on 17-10-2.
//

#ifndef SB_SERVER_SB_RESPONSE_BUILD_FILTER_H
#define SB_SERVER_SB_RESPONSE_BUILD_FILTER_H

#include <sb_collection.h>
#include <stddef.h>
#include "sb_request_parse_filter.h"
#include "sb_static_resource.h"
/**
 * 资源找到时对应的处理链
 * */
static sb_filter_chain *success_response_builder = NULL;

/**
 * 资源没有找到时对应的处理链
 * */
static sb_filter_chain *fail_response_builder = NULL;

int sb_set_response_builder(sb_filter_chain *success_res_builder, sb_filter_chain *fail_res_builder);

int sb_add_filter_success_response_builder(FILTER);

int sb_add_filter_fail_response_builder(FILTER);

int sb_init_response_builder();

sb_filter_chain* sb_get_success_response_builder();

sb_filter_chain* sb_get_fail_response_builder();

int invoke_success_response_builder(sb_client *client, sb_static_resource *resource);

/**
 * 根据请求的参数来寻找资源
 * 资源包括静态资源也包括函数(action)
 * 优先匹配静态资源
 * */
void sb_build_response(sb_client *client);

#endif