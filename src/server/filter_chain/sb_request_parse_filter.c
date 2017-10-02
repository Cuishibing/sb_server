//
// Created by cui on 17-9-21.
//

#include "sb_server.h"
#include "filter_chain/sb_filter_chain.h"
#include "filter_chain/sb_request_parse_filter.h"

int sb_set_request_parse_filters(sb_filter_chain *filter_chain){
    if(filter_chain == NULL){
        return fail;
    }
    if(request_parse_filter_chain == NULL){
        request_parse_filter_chain = filter_chain;
        return success;
    }
    return fail;
}

int sb_add_method_req_parse_filters(FILTER){
    return sb_add_filter(request_parse_filter_chain,filter);
}

int sb_init_request_parse_filters(){
    if(request_parse_filter_chain == NULL){
        request_parse_filter_chain = (sb_filter_chain*)malloc(sizeof(sb_filter_chain));
        if(request_parse_filter_chain == NULL){
            error("内存不足!");
            return fail;
        }
        return sb_init_filter_chain(request_parse_filter_chain);
    }
    return success;
}

sb_filter_chain* sb_get_request_parse_filters(){
    return request_parse_filter_chain;
}