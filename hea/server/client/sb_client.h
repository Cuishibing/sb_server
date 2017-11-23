//
// Created by cui on 17-9-17.
//

#ifndef SB_SERVER_SB_CLIENT_H
#define SB_SERVER_SB_CLIENT_H

#include <time.h>
#include <stdlib.h>
#include "sb_data_cache.h"
#include "sb_request.h"

/**
 * 定义连接的状态
 * */
typedef enum{
    INITED,//初始化完毕
    READING,//正在读取请求
    HANDING,//正在处理
    WRITTING,//正在返回响应
    OVER //结束
}clienter_status;

/**
 * 定义客户端的连接
 * */
typedef struct {
    int socket_fd;
    time_t *time_stamp;//时间戳
    clienter_status status;
    sb_data_cache *data_cache;
    sb_request *request;
}sb_client;

/**
 * todo:因为目前没有实现管理连接的模块,所以这里暂时用一个数组实现
 * */
sb_client* clients[100];

/**
 * 根据socket从连接管理模块中获取client对象
 * @param socket client的socket
 * @return 缓存中的client,如果还没有缓存返回NULL
 * */
sb_client* sb_get_client(int socket);

/**
 * 从连接管理模块中移除一个client
 * */
void sb_remove_client(sb_client *client);

/**
 * 根据socket初始化一个连接
 * @param client 要初始化的连接对象
 * @param socket socket id
 * @return success 成功,fail 失败
 * */
int sb_init_client(sb_client *client, int socket);

/**
 * 更新一个连接的状态
 * @param client 要更新的连接对象
 * @param status 状态
 * @return success 成功,fail 失败
 * */
int sb_touch_client(sb_client *client, clienter_status status);

#endif //SB_SERVER_SB_CLIENT_H