//
// Created by cui on 17-9-16.
//

#include <sb_server.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sb_connection.h>
#include <unistd.h>
#include <sb_client.h>
#include <stdlib.h>
#include <sb_read_worker.h>
#include <sb_data_cache.h>
#include <sb_write_worker.h>
#include "sb_dispatcher.h"

int sb_start_dispatcher(){
    struct epoll_event ev;
    sb_server *server = sb_get_server();
    ev.events = EPOLLIN;
    ev.data.fd = server->server_socket;
    if(fail == sb_add_epoll_event(server->server_socket,&ev)){
        return fail;
    }

    int connect_socket = -1;
    while(!server->is_exit){
        int events_nums = epoll_wait(server->epoll_fd,server->events,server->max_events_nums, -1);
        if(events_nums < 0){
            error("epoll wait失败!\n");
            return fail;
        }
        for(int i=0;i<events_nums;i++){
            if(server->events[i].data.fd == server->server_socket){
                connect_socket = accept(server->server_socket,NULL,NULL);
                if(connect_socket == -1){
                    error("无效的connect socket！\n");
                    break;
                }else{
                    if(fail == sb_set_socket_noblock(connect_socket)){
                        error("设置非锁socket失败!\n");
                        break;
                    }
                    ev.events = EPOLLIN | EPOLLET;//可读事件，边缘触发
                    ev.data.fd = connect_socket;
                    if(fail == sb_add_epoll_event(connect_socket,&ev)){
                        break;
                    }else{
                        if(server->large_clienters>0){
                            server->large_clienters--;
                        }else{
                            close(connect_socket);
                        }
                    }
                }
            }else{
                switch (server->events[i].events){
                    case EPOLLIN:{//可读
                        sb_client *client = sb_get_client(server->events[i].data.fd);
                        if(client == NULL){
                            client = (sb_client*)malloc(sizeof(sb_client));
                            if(client == NULL){
                                error("内存不足!\n");
                                return fail;
                            }
                            sb_init_client(client,server->events[i].data.fd);
                        }
                        sb_add_read_event(client);
                    }break;
                    case EPOLLOUT:{//可写
                        sb_client *client = sb_get_client(server->events[i].data.fd);
                        if(client != NULL){
                            //fprintf(stderr,client->data_cache->data_poll);
                            sb_add_write_event(client);
                        }
                    }break;
                }
            }
        }
    }
    return success;
}

int sb_add_epoll_event(int fd,struct epoll_event *ev){
    sb_server *server = sb_get_server();
    if (epoll_ctl(server->epoll_fd, EPOLL_CTL_ADD, fd,ev) == -1){
        error("注册epoll事件失败!\n");
        return fail;
    }
    return success;
}

int sb_mod_epoll_event(int fd, struct epoll_event *ev){
    sb_server *server = sb_get_server();
    if (epoll_ctl(server->epoll_fd, EPOLL_CTL_MOD, fd,ev) == -1){
        error("改变epoll事件失败!\n");
        return fail;
    }
    return success;
}