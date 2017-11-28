//
// Created by cui on 17-9-23.
//

#ifndef SB_SERVER_SB_HTTP_H
#define SB_SERVER_SB_HTTP_H

#include "sb_client.h"
#include "sb_request.h"

/**
 * 这部分并不是sb_server中的内容,在sb_server的基础上实现HTTP服务器需要提供相应的请求解析模块和响应构建模块.
 * 定义http协议的解析和响应的构建
 * */

void sb_register_http_model();

static const char *REQUEST_METHOD = "request_method";

static const char *REQUEST_URL = "request_url";

static const char *REQUEST_VERSION = "request_version";

static const char *REQUEST_HEADS = "request_headers";

#endif //SB_SERVER_SB_HTTP_H