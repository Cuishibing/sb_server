//
// Created by cui on 17-9-21.
//

#include "sb_server.h"
#include "filter_chain/sb_filter_chain.h"
#include "filter_chain/sb_request_parse_filter.h"

int sb_set_request_builder(sb_filter_chain *filter_chain){
    if(filter_chain == NULL){
        return fail;
    }
    if(request_builder != NULL){
        free(request_builder);
    }
    request_builder = filter_chain;
    return success;
}

int sb_add_filter_request_builder(FILTER){
    return sb_add_filter(request_builder,filter);
}

int sb_init_request_builder(){
    if(request_builder == NULL){
        request_builder = (sb_filter_chain*)malloc(sizeof(sb_filter_chain));
        if(request_builder == NULL){
            error("内存不足!");
            return fail;
        }
        return sb_init_filter_chain(request_builder);
    }
    return success;
}

sb_filter_chain* sb_get_request_builder(){
    return request_builder;
}

int invoke_request_builder(sb_client *client){
    if(client == NULL){
        return fail;
    }
    sb_filter_chain *request_parse_filter = sb_get_request_builder();
    void *result_arg = NULL;
    result_arg = sb_invoke_filter_chain(request_parse_filter,client,result_arg);
    return result_arg == NULL ? fail : success;
}