//
// Created by cui on 17-9-23.
//

#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include "sb_server.h"
#include "sb_static_resource.h"

/**
 * 一个获取本地资源类,同时能够对获取到的资源进行缓存
 * */
static sb_map *sb_resource_cache = NULL;

sb_static_resource* new_static_resource(const char *path, const char *name){
    size_t length_name = strlen(name);
    FILE *resource_file = fopen(path,"r");
    if(resource_file == NULL){
        error("资源打开失败!\n");
        return NULL;
    }
    sb_static_resource *result = (sb_static_resource*)malloc(sizeof(sb_static_resource));
    if(result == NULL){
        error("内存不足!\n");
        return NULL;
    }else{
        if(fail == sb_init_data_cache(&result->data)){
            return NULL;
        }
        result->name = (char*)malloc(length_name + 1);
        if(result->name == NULL){
            error("内存不足!\n");
            return NULL;
        }
        strcpy(result->name,name);
        result->super = sb_new_resource(STATIC,result->name);
    }
    char buffer[BUFFER_SIZE];
    size_t length = 0;
    length = fread(buffer,sizeof(char),BUFFER_SIZE - 1,resource_file);
    while(length != 0 && length != -1){
        buffer[length] = '\0';
        if(success == sb_put_data_cache(&result->data,buffer)){
            length = fread(buffer,sizeof(char),BUFFER_SIZE - 1,resource_file);
        }else{
            error("读取资源错误!\n");
            return NULL;
        }
    }
    fclose(resource_file);
    return result;
}

sb_static_resource* sb_find_static_resource(const char *name){
    sb_server *server = sb_get_server();
    const char *root_path = sb_get_context_property(server->context,ROOT_PATH);

    if(root_path == NULL || name == NULL){
        error("bad super name and path!\n");
        return NULL;
    }

    sb_element element;
    if(sb_resource_cache != NULL){
        size_t length_root_path = strlen(root_path);
        size_t length_name = strlen(name);
        char *resource_path = (char*)calloc(length_name + length_root_path + 1,sizeof(char));
        strcat(resource_path,root_path);
        strcat(resource_path,name);
        if(fail == sb_get_map(sb_resource_cache,resource_path,(length_name + length_root_path),&element)){
            //还没有缓存
            sb_static_resource* resource = new_static_resource(resource_path, name);
            if(resource == NULL){
                return NULL;
            }else{
                //缓存资源
                element.value = resource;
                sb_insert_map(sb_resource_cache,resource_path,(length_name + length_root_path),element);
                return resource;
            }
        }else{
            free(resource_path);
            return (sb_static_resource*)element.value;
        }
    }else{
        sb_resource_cache = (sb_map*)malloc(sizeof(sb_map));
        if(sb_resource_cache == NULL){
            error("内存不足!\n");
            return NULL;
        }else{
            if(success == sb_init_map(sb_resource_cache)){
                return sb_find_static_resource(name);
            }else{
                error("初始化map失败!\n");
                return NULL;
            }
        }
    }
}