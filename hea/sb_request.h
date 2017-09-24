//
// Created by cui on 17-9-20.
//

#ifndef SB_SERVER_SB_REQUEST_H
#define SB_SERVER_SB_REQUEST_H

#include "sb_key_value.h"

typedef struct {
    sb_key_value request_data;//store request parameters.
    sb_key_value attribute_data;//用于下一步构建响应.
}sb_request;

int sb_init_request(sb_request *request);

char* sb_get_request_parameter(const sb_request *request, const char *name);

int sb_set_request_parameter(const sb_request *request, const char *name,const char *value);

int sb_set_request_attribute(const sb_request *request, const char *name, const char *value);

char* sb_get_request_attribute(const sb_request *request,const char *name);

#endif //SB_SERVER_SB_REQUEST_H
