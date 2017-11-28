//
// Created by cui on 17-11-26.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <data/sb_key_value.h>
#include "sb_server.h"

#include "sb_dynamic_resource.h"

static sb_key_value *sb_resource_cache = NULL;

sb_dynamic_resource* sb_new_dynamic_resource(const char* resource_name,void(*method_ptr(void*))){
    if(resource_name == NULL || method_ptr == NULL)
        return NULL;
    if(strcmp(resource_name,"")==0)
        return NULL;
    sb_dynamic_resource *dy_resource = (sb_dynamic_resource*)malloc(sizeof(sb_dynamic_resource));
    if(dy_resource == NULL){
        error("内存不足!");
        return NULL;
    }
    dy_resource->super = sb_new_resource(DYNAMIC,resource_name);
    if(dy_resource->super == NULL){
        error("sb_resource构建指失败!");
        return NULL;
    }
    dy_resource->method_ptr = method_ptr;
    return dy_resource;
}

sb_dynamic_resource* sb_find_dynamic_resource(const char* resource_name){
    if(resource_name == NULL || (strcmp("",resource_name)==0))
        return NULL;
    if(sb_resource_cache == NULL){
        error("dynamic_resource_holder没有初始化!");
        return NULL;
    }
    sb_element element;
    if(success == sb_get_map(sb_resource_cache, (void *) resource_name, strlen(resource_name), &element)){
        return (sb_dynamic_resource*)element.value;
    }
    return NULL;
}

int sb_add_dynamic_resource(const sb_dynamic_resource *dy_resource){
    if(dy_resource == NULL)
        return fail;
    const char* resource_name = dy_resource->super->resource_name;
    if(sb_resource_cache == NULL){
        sb_resource_cache = (sb_key_value*)malloc(sizeof(sb_key_value));
        if(sb_resource_cache == NULL){
            error("内存不足!");
            return fail;
        }
        if(success == sb_init_key_value(sb_resource_cache)){
            return sb_put_key_value(sb_resource_cache,resource_name,dy_resource);
        }
    }else{
        sb_arraylist key_set = sb_resource_cache->key_set;
        sb_element element;
        for(int i=0;i<key_set.length;i++){
            sb_get_arraylist(&key_set,i,&element);
            if(strcmp(resource_name,(char*)element.value) == 0){
                error("该动态资源已经存在!");
                return fail;
            }
        }
        return sb_put_key_value(sb_resource_cache,resource_name,dy_resource);
    }
    return fail;
}