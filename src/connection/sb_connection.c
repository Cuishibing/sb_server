//
// Created by cui on 17-9-13.
//
#include <netinet/in.h>
#include <stdio.h>
#include <wchar.h>
#include <memory.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sb_server.h>
#include <unistd.h>
#include "sb_connection.h"
int sb_init_server_socket(int port,const char *addr) {
    int server_socket_id = -1;
    struct sockaddr_in server_addr;
    //初始化server_socket
    if ((server_socket_id = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        error("初始化server_socket出错!\n");
        return fail;
    }

    if (sb_init_sockaddr_in(addr, &server_addr, port) == fail)
        return fail;

    //把server_addr绑定到server_socket上
    if ((bind(server_socket_id, (struct sockaddr*) &server_addr,
              sizeof(server_addr))) == -1) {
        error("绑定server_addr到server_socket出错!\n");
        return fail;
    }
    return server_socket_id;
}

int sb_init_client_socket() {
    int client_socket_id = -1;
    struct sockaddr_in client_addr;
    if (sb_init_sockaddr_in(NULL, &client_addr, NULL) == fail)
        return fail;
    //初始化client_socket
    if ((client_socket_id = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        error("初始化client_socket出错!\n");
        return fail;
    }
    //把client_socket绑定到client_addr
    if ((bind(client_socket_id, (struct sockaddr*) &client_addr,
              sizeof(client_addr))) == -1) {
        error("绑定client_addr到client_socket出错!\n");
        return fail;
    }
    return client_socket_id;
}

int sb_init_sockaddr_in(const char *addr, struct sockaddr_in *sock_addr, int port) {
    //初始化sock_addr
    memset(sock_addr, 0, sizeof(struct sockaddr_in));
    sock_addr->sin_family = AF_INET;
    if (port != 0)
        sock_addr->sin_port = htons(port);
    else
        sock_addr->sin_port = htons(0);
    if (addr != NULL) {
        if (inet_pton(PF_INET, addr, &sock_addr->sin_addr) <= 0) {
            error("inet_pton出错!\n");
            return fail;
        }
    } else
        sock_addr->sin_addr.s_addr = htonl(INADDR_ANY); //分配本机地址
    return success;
}

int sb_set_socket_noblock(int socket_id) {
    /*
     * 方法一:fcntl
     int flag;
     if (flag = fcntl(fd, F_GETFL, 0) <0) perror("get flag");
     flag |= O_NONBLOCK;
     if (fcntl(fd, F_SETFL, flag) < 0)
     perror("set flag");
     方法二:ioctl
     int b_on = 1;
     ioctl (fd, FIONBIO, &b_on);
     * */
    int flag;
    flag = fcntl(socket_id, F_GETFL, 0);
    if (flag < 0) {
        fprintf(stderr,"fcntl get failed!\n");
        return 0;
    }
    flag |= O_NONBLOCK;
    if (fcntl(socket_id, F_SETFL, flag) < 0) {
        fprintf(stderr,"fcntl set failed!\n");
        return 0;
    }
    return 1;
}
