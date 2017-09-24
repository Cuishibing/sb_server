//
// Created by cui on 17-9-17.
//
#include <stdlib.h>
#include <stdio.h>
#include <sb_data_cache.h>
#include <assert.h>
#include <memory.h>
#include "sb_data_cache.h"

int sb_init_data_cache(sb_data_cache *cache){
    if(cache == NULL){
        fprintf(stderr,"初始化data_cache失败!");
        return 0;
    }
    cache->length = 0;
    cache->size = DEFAULT_SIZE;//默认是2KB
    cache->data_poll = (char*)calloc(cache->size,sizeof(char));
    if(cache->data_poll == NULL){
        fprintf(stderr,"内存不足!");
        return 0;
    }
    return 1;
}

static int expand_cache_size(sb_data_cache *cache){
    assert(cache != NULL);
    size_t new_size = cache->size + cache->size / 2;
    cache->data_poll = realloc(cache->data_poll,new_size);
    if(cache->data_poll == NULL){
        fprintf(stderr,"内存不足!");
        return 0;
    }
    cache->size = new_size;
    return 1;
}

int sb_put_data_cache(sb_data_cache *cache,char *buffer){
    if(cache == NULL || buffer == NULL){
        return 0;
    }
    size_t buffer_len = strlen(buffer);
    while(cache->length + buffer_len > cache->size){//需要扩展
        if(!expand_cache_size(cache)){
            return 0;
        }
    }
    char *start_point = &(cache->data_poll[cache->length]);
    if(start_point != NULL){
        memcpy(start_point,buffer,buffer_len);
        cache->length += buffer_len;
    }else{
        return 0;
    }
    return 1;
}

int sb_clear_data_cache(sb_data_cache *cache){
    if(cache == NULL){
        return 0;
    }
    cache->length = 0;
    cache->size = DEFAULT_SIZE;
    cache->data_poll = (char*)realloc(cache->data_poll,cache->size);
    if(cache->data_poll == NULL){
        fprintf(stderr,"clear data_cache失败!");
        return 0;
    }
    return 1;
}

int sb_trim_data_cache(sb_data_cache *cache){
    if(cache == NULL){
        return 0;
    }
    cache->size = cache->length;
    cache->data_poll = (char*)realloc(cache->data_poll,cache->size);
    if(cache->data_poll == NULL){
        fprintf(stderr,"trim data_cache失败!");
        return 0;
    }
    return 1;
}