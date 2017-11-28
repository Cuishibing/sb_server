//
// Created by cui on 17-11-27.
//

#ifndef SB_SERVER_SB_RESPONSE_BUILDER_H
#define SB_SERVER_SB_RESPONSE_BUILDER_H

#include <server/resource/sb_resource.h>
#include "sb_request_parser.h"

typedef struct response_builder_method{
    int(*method_ptr)(void*,sb_resource**);
}response_builder;

response_builder* sb_get_response_builder();

response_builder* sb_new_response_builder(int(*method_ptr)(void *,sb_resource**));

/**
 * 设置响应构建的接口
 * @param method_ptr 响应构建所使用的函数指针
 * @return 是否设置成功
 * */
int sb_set_response_builder(int(*method_ptr)(void *, sb_resource **));

#endif //SB_SERVER_SB_RESPONSE_BUILDER_H
