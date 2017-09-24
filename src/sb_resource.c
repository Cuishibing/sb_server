//
// Created by cui on 17-9-23.
//

#include <sb_resource.h>
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <unistd.h>

sb_resource* init_resource(const char* path,const char* name){
    size_t length_name = strlen(name);
    FILE *resource_file = fopen(path,"r");
    if(resource_file == NULL){
        fprintf(stderr,"资源打开失败!");
        return NULL;
    }
    sb_resource *result = (sb_resource*)malloc(sizeof(sb_resource));
    if(result == NULL){
        fprintf(stderr,"内存不足!");
        return NULL;
    }else{
        if(!sb_init_data_cache(&result->data)){
            return NULL;
        }
        result->name = (char*)malloc(length_name + 1);
        if(result->name == NULL){
            fprintf(stderr,"内存不足!");
            return NULL;
        }
        strcpy(result->name,name);
    }
    char buffer[BUFFER_SIZE];
    size_t length = 0;
    length = fread(buffer,sizeof(char),BUFFER_SIZE - 1,resource_file);
    while(length != 0 && length != -1){
        if(sb_put_data_cache(&result->data,buffer)){
            length = fread(buffer,sizeof(char),BUFFER_SIZE - 1,resource_file);
        }else{
            fprintf(stderr,"读取资源错误!");
            return NULL;
        }
    }
    fclose(resource_file);
    return result;
}

sb_resource* sb_get_resource(const char* root_path,const char* name){
    if(root_path == NULL || name == NULL){
        fprintf(stderr,"bad resource name and path!");
        return NULL;
    }
    sb_element element;
    if(sb_resource_cache != NULL){
        size_t length_root_path = strlen(root_path);
        size_t length_name = strlen(name);
        char *resource_path = (char*)malloc(length_name + length_root_path + 1);
        char *temp = strcat(resource_path,root_path);
        strcat(temp,name);
        if(!sb_get_map(sb_resource_cache,resource_path,(length_name + length_root_path),&element)){
            //还没有缓存
            sb_resource* resource = init_resource(resource_path,name);
            if(resource == NULL){
                return NULL;
            }else{
                element.value = resource;
                sb_insert_map(sb_resource_cache,resource_path,(length_name + length_root_path),element);
                return resource;
            }
        }else{
            return (sb_resource*)element.value;
        }
    }else{
        sb_resource_cache = (sb_map*)malloc(sizeof(sb_map));
        if(sb_resource_cache == NULL){
            fprintf(stderr,"内存不足!");
            return NULL;
        }else{
            if(sb_init_map(sb_resource_cache)){
                return sb_get_resource(root_path,name);
            }else{
                fprintf(stderr,"初始化map失败!");
                return NULL;
            }
        }
    }
}