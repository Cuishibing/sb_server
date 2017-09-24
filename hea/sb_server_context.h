//
// Created by cui on 17-9-16.
//

#ifndef SB_SERVER_SB_INITSERVER_H
#define SB_SERVER_SB_INITSERVER_H

#include "sb_key_value.h"

static const char *MAX_EPOLL_EVENTS_NUMS = "max_epoll_events_nums";
static const char *READ_WORKERS_NUMS = "read_workers_nums";
static const char *HANDLE_WORKERS_NUMS = "handle_works_nums";
static const char *WRITE_WORKERS_NUMS = "write_workers_nums";
static const char *LARGE_CLIENTERS_NUMS = "large_clienters_nums";
static const char *BACK_LOG = "back_log";

static const char *ROOT_PATH = "root_path";

typedef struct server_context{
    sb_key_value properties;
}sb_server_context;
/*
 * 初始化server_context
 * 成功：非0
 * 失败：0
 * */
int sb_init_server_context(sb_server_context *context);

/*
 * 设置一个属性
 * */
int sb_put_context_property(const sb_server_context *context,const char *name, const char *value);

/*
 * 根据属性名获取一个属性
 * 返回属性值，如果没有这个属性则返回NULL
 * */
char* sb_get_context_property(const sb_server_context *context,const char *name);

#endif //SB_SERVER_SB_INITSERVER_H
