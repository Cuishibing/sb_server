//
// Created by cui on 17-9-16.
//
#include <stdlib.h>
#include <stdio.h>
#include <sb_server.h>
#include <sys/socket.h>
#include <sb_dispatcher.h>
#include <sb_read_worker.h>
#include <sb_handle_worker.h>
#include <unistd.h>
#include "sb_connection.h"

int sb_init_server(const sb_server_context *context, int port){
    if(context == NULL || port <= 0){
        return fail;
    }
    if(server == NULL){
        server = (sb_server*)malloc(sizeof(sb_server));
        if(server == NULL){
            error("内存不足!\n");
            return fail;
        }
        server->is_exit = 0;
        server->context = context;
        server->port = port;

        char *property_str = NULL;

        property_str = sb_get_context_property(server->context,MAX_EPOLL_EVENTS_NUMS);
        int max_epoll_events_num = 20;
        if(property_str != NULL){
            max_epoll_events_num = atoi(property_str);
        }
        server->events = (struct epoll_event*)malloc(sizeof(max_epoll_events_num));
        if(server->events == NULL){
            error("内存不足!\n");
            return fail;
        }
        server->max_events_nums = max_epoll_events_num;

        property_str = NULL;

        int large_clienters_num = 100;
        property_str = sb_get_context_property(server->context,LARGE_CLIENTERS_NUMS);
        if(property_str != NULL){
            large_clienters_num = atoi(property_str);
        }
        server->large_clienters = large_clienters_num;

        property_str = NULL;

        int back_log = 4;
        property_str = sb_get_context_property(server->context,BACK_LOG);
        if(property_str != NULL){
            back_log = atoi(property_str);
        }
        server->back_log = back_log;

        property_str = NULL;
        property_str = sb_get_context_property(server->context,ROOT_PATH);
        if(property_str != NULL){
            server->root_path = property_str;
        }else{
            server->root_path = "./";
        }

        //TODO:初始化其它部件
        sb_read_worker *read_worker = (sb_read_worker*)malloc(sizeof(sb_read_worker));
        sb_init_read_worker(read_worker);

        sb_handler_worker *handler_worker = (sb_handler_worker*)malloc(sizeof(sb_handler_worker));
        sb_init_handle_worker(handler_worker);
        return success;

    }
}

int sb_start_server(){
    if(server == NULL){
        error("server is null!\n");
        return fail;
    }
    server->server_socket = sb_init_server_socket(server->port,NULL);
    if(server->server_socket == fail){
        error("socket获取失败!\n");
        return fail;
    }
    if(fail == sb_set_socket_noblock(server->server_socket)){
        error("设置非锁socket失败!\n");
        close(server->server_socket);
        return fail;
    }
    if(listen(server->server_socket,server->back_log) < 0){
        error("listen失败!\n");
        close(server->server_socket);
        return fail;
    }
    server->epoll_fd = epoll_create1(0);
    if(server->epoll_fd < 0){
        error("初始化epoll失败!\n");
        close(server->server_socket);
        return fail;
    }
    if(fail == sb_start_dispatcher()){
        error("启动dispatcher失败!\n");
        close(server->server_socket);
        return fail;
    }
}

sb_server* sb_get_server(){
    return server;
}