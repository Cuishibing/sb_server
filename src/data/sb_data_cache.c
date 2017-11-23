//
// Created by cui on 17-9-17.
//
#include <stdlib.h>
#include <stdio.h>
#include <sb_data_cache.h>
#include <assert.h>
#include <memory.h>
#include <sb_server.h>
#include <data/sb_data_cache.h>
#include "sb_data_cache.h"

int sb_init_data_cache(sb_data_cache *cache){
    if(cache == NULL){
        error("初始化data_cache失败!\n");
        return fail;
    }
    cache->length = 0;
    cache->size = DEFAULT_SIZE;
    cache->data_poll = (char*)calloc(cache->size,sizeof(char));
    if(cache->data_poll == NULL){
        error("内存不足!\n");
        return fail;
    }
    return success;
}

static int expand_cache_size(sb_data_cache *cache){
    assert(cache != NULL);
    size_t new_size = cache->size + cache->size / 4;
    cache->data_poll = realloc(cache->data_poll,new_size);
    if(cache->data_poll == NULL){
        error("内存不足!\n");
        return fail;
    }
    cache->size = new_size;
    return success;
}

int sb_put_data_cache(sb_data_cache *cache,const char *buffer){
    if(cache == NULL || buffer == NULL){
        return fail;
    }
    size_t buffer_len = strlen(buffer);
    while(cache->length + buffer_len >= cache->size){//需要扩展
        if(fail == expand_cache_size(cache)){
            return fail;
        }
    }
    char *start_point = &(cache->data_poll[cache->length]);
    if(start_point != NULL){
        memcpy(start_point,buffer,buffer_len);
        cache->length += buffer_len;
        cache->data_poll[cache->length] = '\0';
    }else{
        return fail;
    }
    return success;
}

int sb_clear_data_cache(sb_data_cache *cache){
    if(cache == NULL){
        return fail;
    }
    cache->length = 0;
    cache->size = DEFAULT_SIZE;
    cache->data_poll = (char*)realloc(cache->data_poll,cache->size);
    if(cache->data_poll == NULL){
        error("clear data_cache失败!\n");
        return fail;
    }
    return success;
}

int sb_trim_data_cache(sb_data_cache *cache){
    if(cache == NULL){
        return fail;
    }
    cache->size = cache->length + 1;
    cache->data_poll = (char*)realloc(cache->data_poll,cache->size);
    cache->data_poll[cache->length] = '\0';
    if(cache->data_poll == NULL){
        error("trim data_cache失败!\n");
        return fail;
    }
    return success;
}

int sb_free_data_cache(sb_data_cache *data_cache){
    free(data_cache->data_poll);
    return success;
}