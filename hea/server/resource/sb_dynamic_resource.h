//
// Created by cui on 17-11-26.
//

#ifndef SB_SERVER_SB_DENAMIC_RESOURCE_H
#define SB_SERVER_SB_DENAMIC_RESOURCE_H

#include <sb_tree.h>
#include <data/sb_key_value.h>
#include "sb_resource.h"

/************************
 * 定义动态资源模块,能够对资源进行缓存
 ***********************/



typedef struct dynamic_resource{
    sb_resource* super;//父类引用
    void (*method_ptr)(void*);//动态资源所对应的方法指针
}sb_dynamic_resource;

/**
 * 创建一个动态资源
 * @param resource_name 资源名
 * @param method_ptr 方法指针
 * @return 新创建的资源,如果创建失败则返回NULL
 * */
sb_dynamic_resource* sb_new_dynamic_resource(const char* resource_name,void(*method_ptr(void*)));

/**
 * 根据资源名查找一个动态资源
 * @param resource_name 资源名
 * @return 根据名称所找到的资源,如果失败则返回NULL
 * */
sb_dynamic_resource* sb_find_dynamic_resource(const char* resource_name);

/**
 * 添加一个动态资源
 * @param dy_resource 要添加的动态资源
 * @return 是否成功
 * */
int sb_add_dynamic_resource(const sb_dynamic_resource *dy_resource);

#endif //SB_SERVER_SB_DENAMIC_RESOURCE_H
