//
// Created by cui on 17-9-23.
//

#ifndef SB_SERVER_SB_STATIC_RESOURCE_H
#define SB_SERVER_SB_STATIC_RESOURCE_H

#include <sb_tree.h>
#include "sb_data_cache.h"
/**
 * 一个获取本地资源类,同时能够对获取到的资源进行缓存
 * */
static sb_map *sb_resource_cache = NULL;

static const size_t BUFFER_SIZE = 512;

typedef struct static_resource{
    char *name;
    sb_data_cache data;
}sb_static_resource;

/**
 * 根据资源名获取资源,第一次会进行文件的读取
 * @param name 资源的名称(文件名),加上根路径才是完整的文件路径
 * @return 资源名对应的资源
 * */
sb_static_resource* sb_get_static_resource(const char *name);

#endif