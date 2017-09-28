//
// Created by cui on 17-9-17.
//

#ifndef SB_SERVER_SB_CLIENT_H
#define SB_SERVER_SB_CLIENT_H

#include <time.h>
#include <stdlib.h>
#include "sb_data_cache.h"
#include "sb_request.h"

typedef enum{
    INITED,//初始化完毕
    READING,//正在读取请求
    HANDING,//正在处理
    WRITTING,//正在返回响应
    OVER //结束
}clienter_status;

typedef struct {
    int socket_fd;
    time_t *time_stamp;//时间戳
    clienter_status status;
    sb_data_cache *data_cache;
    sb_request *request;
}sb_client;

sb_client* clients[100];

sb_client* sb_get_client(int socket);

int sb_init_client(sb_client *client, int socket);

int sb_touch_client(sb_client *client, clienter_status status);

#endif //SB_SERVER_SB_CLIENT_H
