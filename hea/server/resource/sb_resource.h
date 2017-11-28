//
// Created by cui on 17-11-26.
//

#ifndef SB_SERVER_RESOURCE_BINDER_H
#define SB_SERVER_RESOURCE_BINDER_H

/**********************************
 * 定义根据请求的目标资源寻找资源的模块
 *********************************/

/**
 * 资源类型
 * */
typedef enum generic_resource_type{
    STATIC,//静态资源
    DYNAMIC//动态资源
}sb_resource_type;

/**
 * 通用资源对象
 * */
typedef struct generic_resource{
    sb_resource_type type;
    char* resource_name;
}sb_resource;

/**
 * 新建一个资源
 * @param type 资源的类型
 * @param resource_name 资源的名称
 * @return 成功返回资源,失败返回NULL
 * */
sb_resource* sb_new_resource(sb_resource_type type,const char* resource_name);

/**
 * 根据资源的名称寻找资源
 * @param target_name 资源名
 * @return 成功返回找到的资源,失败返回NULL
 * */
sb_resource** sb_find_resource(const char* target_name);

#endif //SB_SERVER_RESOURCE_BINDER_H
