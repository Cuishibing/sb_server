//
// Created by cui on 17-9-16.
//

#ifndef SB_SERVER_SB_SERVER_H
#define SB_SERVER_SB_SERVER_H

#include <sys/epoll.h>
#include "sb_server_context.h"
typedef struct {
    const sb_server_context *context;//代表了各种参数
    int server_socket;//一个服务的socket
    int port;//端口号
    int epoll_fd;
    struct epoll_event *events;
    int max_events_nums;
    long large_clienters;
    int back_log;
    int is_exit;

    const char *root_path;

}sb_server;

static sb_server *server = NULL;

int sb_init_server(const sb_server_context *context, int port);

int sb_start_server();

sb_server* sb_get_server();

#endif //SB_SERVER_SB_SERVER_H
