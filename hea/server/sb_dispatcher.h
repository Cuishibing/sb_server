//
// Created by cui on 17-9-16.
//

#ifndef SB_SERVER_SB_DISPATCHER_H
#define SB_SERVER_SB_DISPATCHER_H

#include "sb_server.h"

int sb_start_dispatcher();

int sb_add_epoll_event(int fd,struct epoll_event *ev);

int sb_mod_epoll_event(int fd, struct epoll_event *ev);

int sb_remove_epoll_event(int fd,struct epoll_event *ev);

#endif //SB_SERVER_SB_DISPATCHER_H
