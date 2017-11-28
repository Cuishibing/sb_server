//
// Created by cui on 17-11-27.
//
#include <stdlib.h>
#include "sb_server.h"
#include "sb_response_builder.h"
/**
 * 请求解析的接口
 * */
static response_builder* response_builder_interface = NULL;

int sb_set_response_builder(int(*method_ptr)(void *, sb_resource **)){
    if(method_ptr != NULL){
        if(response_builder_interface == NULL){
            response_builder_interface = sb_new_response_builder(method_ptr);
        }else{
            response_builder_interface->method_ptr = method_ptr;
        }
        return success;
    }
    return fail;
}

response_builder* sb_new_response_builder(int(*method_ptr)(void *,sb_resource**)){
    response_builder *result = (response_builder*)malloc(sizeof(response_builder));
    if(result!=NULL){
        result->method_ptr = method_ptr;
    }
    return result;
}

response_builder* sb_get_response_builder(){
    return response_builder_interface;
}