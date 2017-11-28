//
// Created by cui on 17-9-20.
//

#ifndef SB_SERVER_SB_REQUEST_H
#define SB_SERVER_SB_REQUEST_H

#include "sb_key_value.h"

typedef struct {
    sb_key_value request_data;//存放请求的各种参数
}sb_request;

int sb_init_request(sb_request *request);

void* sb_get_request_parameter(const sb_request *request, const char *name);

int sb_set_request_parameter(const sb_request *request, const char *name,const void *value);
#endif //SB_SERVER_SB_REQUEST_H
