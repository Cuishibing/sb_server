//
// Created by cui on 17-9-23.
//

#ifndef SB_SERVER_SB_HTTP_H
#define SB_SERVER_SB_HTTP_H

#include "sb_client.h"
#include "sb_request.h"

/*
 * 定义http协议的解析和响应的构建
 * */

int sb_init_http_filters();

void* sb_parse_http_start(sb_client *client,void *args);

void* sb_parse_http_head(sb_client *client,void *args);

void* sb_parse_http_body(sb_client *client,void *args);

void sb_print_http_success_start(sb_client *client,int code,const char *reason);

void sb_print_http_head(sb_client *client);

static const char *REQUEST_METHOD = "request_method";

static const char *REQUEST_URL = "request_url";

static const char *REQUEST_TARGET = "request_target";

static const char *REQUEST_VERSION = "request_version";

static const char *REQUEST_HEADS = "request_headers";

#endif //SB_SERVER_SB_HTTP_H
