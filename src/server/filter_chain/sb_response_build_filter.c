// Created by cui on 17-10-2.
//
#include <data/sb_static_resource.h>
#include <http/sb_http.h>
#include "sb_server.h"
#include "filter_chain/sb_response_build_filter.h"

int sb_set_response_builder(sb_filter_chain *success_res_builder, sb_filter_chain *fail_res_builder){
    if(success_res_builder == NULL || fail_res_builder == NULL){
        return fail;
    }
    if(success_response_builder != NULL){
        free(fail_response_builder);
    }
    if(fail_response_builder == NULL){
        free(fail_response_builder);
    }
    success_response_builder = success_res_builder;
    fail_response_builder = fail_res_builder;
    return success;
}

int sb_add_filter_success_response_builder(FILTER){
    return sb_add_filter(success_response_builder,filter);
}

int sb_add_filter_fail_response_builder(FILTER){
    return sb_add_filter(fail_response_builder,filter);
}

int sb_init_response_builder(){
    if(success_response_builder == NULL){
        success_response_builder = (sb_filter_chain*)malloc(sizeof(sb_filter_chain));
        if(success_response_builder == NULL){
            error("内存不足!");
            return fail;
        }
    }
    if(fail_response_builder == NULL){
        fail_response_builder = (sb_filter_chain*)malloc(sizeof(sb_filter_chain));
        if(fail_response_builder == NULL){
            error("内存不足!");
            return fail;
        }
    }
    return sb_init_filter_chain(success_response_builder)
           && sb_init_filter_chain(fail_response_builder);
}

sb_filter_chain* sb_get_success_response_builder(){
    return success_response_builder;
}

sb_filter_chain* sb_get_fail_response_builder(){
    return fail_response_builder;
}

void sb_build_response(sb_client *client){
    char *target = sb_get_request_parameter(client->request,REQUEST_TARGET);
    if(target != NULL){
        sb_static_resource *target_resource = sb_get_static_resource(target);
        if(target_resource != NULL){
            //找到了静态资源
            invoke_success_response_builder(client, target_resource);
            return;
        }else{
            //TODO:继续找动态资源,动态资源
            invoke_success_response_builder(client, target_resource);
            return;
        }
    }
}

int invoke_success_response_builder(sb_client *client, sb_static_resource *resource){
    if(client == NULL){
        return fail;
    }
    sb_filter_chain *success_response_builder = sb_get_success_response_builder();
    void *result_arg = NULL;
    result_arg = sb_invoke_filter_chain(success_response_builder,client,resource);
    return result_arg == NULL ? fail : success;
}