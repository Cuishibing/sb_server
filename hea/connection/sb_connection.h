//
// Created by cui on 17-9-13.
//

#ifndef SB_SERVER_SB_CONNECTION_H
#define SB_SERVER_SB_CONNECTION_H
/*
 * 初始化一个服务器端的socket
 * port:要监听的端口号
 * addr:点分十进制形式的ip地址，如果addr为NULL则自动分配一个本机地址
 * return:成功 返回socket的id
 *      失败 返回fail
 * */
int sb_init_server_socket(int port,const char *addr);

/*
 * 初始化一个客户端的socket
 * 返回socket的id，如果出现错误则返回fail
 * */
int sb_init_client_socket();

/*
 * 初始化一个struct sockaddr_in对象，并默认设置一些参数
 * addr:点分十进制表示的ip地址，如果addr为空则自动分配地址
 * port:端口号，如果为空则自动分配
 * 出现错误则返回fail,否则返回一个大于0的整数
 * */
int sb_init_sockaddr_in(const char *addr, struct sockaddr_in *sock_addr, int port);

/*
 * 把一个socket设置为非阻塞状态
 * socket_id:待设置的socket的id
 * 出现错误时返回fail,否则返回一个大于0的整数
 * */
int sb_set_socket_noblock(int socket_id);
#endif //SB_SERVER_SB_CONNECTION_H